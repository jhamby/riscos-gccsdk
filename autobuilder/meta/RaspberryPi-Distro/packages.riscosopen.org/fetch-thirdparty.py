#!/usr/bin/env python

# Specification:
#
# For each third party developer:
#   Fetch a manifest file containing a list of relative URLs to packages, viz:
#     /package1.zip
#     /mypackage.zip
#   Download each package and place in a directory on the webserver, eg
#     /var/www/thirdparty/arm/example.com/package1.zip
# Run build-index with the -r (rename) flag: will rename all the packages to meet
#   the naming convention and generate the index

# Security:
# if this is run by cron, is a way for third parties to inject code and exploits into
# the system.
#  New developers and packages to be vetted
#  Manifest file does not contain http://.../ part of URL - script adds this.
#    Exploits like http://example.com/:password/../../evil won't work
#  Packages are fetched into different directories - no way to overwrite someone
#    else's
#  Developers are trusted.  If a developer makes FooApp 9.99 we assume it supersedes
#    previous versions.  Developers can supersede each others' applications

# tuples of (Name, server, manifest location)

import os
import requests
import subprocess
import errno

locations = [("SourceName", "http://www.server.example.com/", "/url/to/manifest/file.txt")]

for site in locations:
  name = site[0]
  baseurl = site[1]
  manifest = site[2]
  
  try:
    os.mkdir(name)
  except OSError as exc:
    if exc.errno == errno.EEXIST and os.path.isdir(name):
      pass
    else: raise
  os.chdir(name)
  
  manifesturl = baseurl + manifest
  print manifesturl;
  r = requests.get(manifesturl)

  for package in r.iter_lines():
    # carefully use subprocess.call so no messing about with argument parsing eg
    # url="http://www.example.com/file ; rm -rf /"
    wget = subprocess.call(["wget","-N",baseurl+package])

  os.chdir("..")
  

subprocess.call(["./build-index"])
