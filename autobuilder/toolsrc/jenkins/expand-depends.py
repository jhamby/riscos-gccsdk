#!/usr/bin/env python
# Walk autobuilder directories expanding the depends files to
# a file fulldepends that includes the dependencies of the
# dependencies.
#

# Also sort packages by their popularity as dependencies
# and output a build order list

import os
import sys
from collections import OrderedDict

package_list = []

depends_dict = {}

inverse_tree = {}

# Expand all the dependcies for the given package
# pkg is the package to expand
# s is the set to add the dependencies to
def expand_deps(pkg, s):
    if depends_dict.has_key(pkg):
       for dep in depends_dict[pkg]["deps"]:
           if not dep in s:
              s.add(dep)
              expand_deps(dep, s)

def tree_deps(pkg, s):
    if depends_dict.has_key(pkg):
       for dep in depends_dict[pkg]["deps"]:
            if pkg not in inverse_tree:
                inverse_tree[pkg] = set()
            inverse_tree[pkg].add(dep)

# sort by length
def compare_count(x, y):
    return (len(x) < len(y))


# Repeatedly scan the tree, looking for packages that have no children (ie have no prerequisites)
# Add those to the build list, then remove them from all the package dependencies that remain.
# That should leave some more with no prerequisities
# Repeat until the tree is empty, or we make no progress (ie the tree is inconsistent)

def iterate_leaves(tree, filestem):
    # iterate until the list is empty
    iterations = 0
    while len(tree)>0:
    # first list all the leaf nodes
        leaves = []
        buildorder = open(filestem+str(iterations),"w")

        for key in tree:
            value = tree[key]
            if (len(value)==0):
                # leaf node, so make a note and remove it from the tree
                buildorder.write(key+"\n")
                print "iter %d: remove %s" % (iterations,key)
                leaves.append(key)
        print "leaves: %s" % (leaves)
        if len(leaves) == 0:
            # deadlock because something went wrong - the graph is disjoint
            break;
        # then remove them from the list itself
        for leaf in leaves:
            del tree[leaf]
        # remove all leaves from the list of deps
        for key in tree:
            deps = tree[key]
            for leaf in leaves:
                if leaf in deps:
                    print "From %s removing dep %s" % (key,leaf)
                    tree[key].remove(leaf)
        # tree now has all traces of leaves removed
        # now iterate for the next run
        iterations = iterations+1
        print "Remaining tree: %s" %(tree)



# Find all dependencies and add to dict
for root, subdirs, files in os.walk("."):
    if "setvars" in files:
        package_list.append(os.path.basename(root))
    for fname in files:
        if fname == "depends":
           with open(os.path.join(root, fname)) as f:
              deps = set(line.strip() for line in f)
              deps.discard("");
              depends_dict[os.path.basename(root)] = {"location": root, "deps": deps}

# Walk list and create full depends file
print package_list
print depends_dict

for key,value in depends_dict.items():
   all_deps = set()
   expand_deps(key, all_deps)
   fname = os.path.join(value["location"],"fulldepends")
   with open(fname, "w") as fd:
      for line in sorted(all_deps):
          fd.write(line+"\n")

# Invert the list, ie create a list of the packages that require a given package
for key,value in depends_dict.items():
    tree_deps(key,value)
# add all those packages that don't have any deps
for package in package_list:
    if (package not in inverse_tree) and (package != '.'):
        inverse_tree[package] = set();

#print inverse_tree
inverse_list={}

for key,value in inverse_tree.items():
    print "%s depends on %d packages: %s" % (key, len(value), value)
    inverse_list[key] = list(value)

print "Inverse dependencies: "
print inverse_list

#sorted_deps = inverse_tree.sort(cmp=compare_count)
sorted_deps = OrderedDict(sorted(inverse_list.items(), key=lambda t: len(t[1])))
print "Sorted list: "
print sorted_deps

iterate_leaves(sorted_deps, "buildorder-iter")

#with open("buildorder","w") as buildorder:
#    while len(sorted_deps)>0:
#        for key,value in sorted_deps:
#            if len(value)==0:
#                buildorder.write(key+"\n")

