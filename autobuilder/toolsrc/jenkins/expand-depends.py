#!/usr/bin/env python
# Walk autobuilder directories expanding the depends files to
# a file fulldepends that includes the dependencies of the
# dependencies.
#

import os
import sys

depends_dict = {}

# Expand all the dependcies for the given package
# pkg is the package to expand
# s is the set to add the dependencies to
def expand_deps(pkg, s):
    if depends_dict.has_key(pkg):
       for dep in depends_dict[pkg]["deps"]:
           if not dep in s:
              s.add(dep)
              expand_deps(dep, s)

# Find all dependencies and add to dict
for root, subdirs, files in os.walk("."):
    for fname in files:
        if fname == "depends":
           with open(os.path.join(root, fname)) as f:
              deps = set(line.strip() for line in f)
              deps.discard("");
              depends_dict[os.path.basename(root)] = {"location": root, "deps": deps}

# Walk list and create full depends file
for key,value in depends_dict.items():
   all_deps = set()
   expand_deps(key, all_deps)
   fname = os.path.join(value["location"],"fulldepends")
   with open(fname, "w") as fd:
      for line in sorted(all_deps):
          fd.write(line+"\n")


