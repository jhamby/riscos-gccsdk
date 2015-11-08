#!/usr/bin/python
# horrifically insecure

from BeautifulSoup import BeautifulSoup
import urllib2
import os
import re
url="https://www.riscosopen.org/content/downloads/raspberry-pi"
page=urllib2.urlopen(url)
soup = BeautifulSoup(page.read())
devel = os.getenv('ROOL_ROM_DEV')
#version = os.getenv('ROOL_ROM_VERSION')
#rc = os.getenv('ROOL_RC')
rc=0
if (devel):
#  filename = "BCM2835Dev.%s.zip" % (version)
  anchor = "beta_rpi_rom"
else:
#  filename = "BCM2835-RC%s.%s.zip" % (rc, version)
  anchor = "beta_rpi_rom__rc"
for link in soup.findAll('a'): # find all links
  try:
    if link['name'] == anchor:
      url=link['href']
    else:
      if 'rc' in anchor:
        if anchor in link['name']:
          rcmatch = re.search("\d+",link['name'])
          if rcmatch:
            newrc = rcmatch.group(0)
            if newrc > rc:
              rc = newrc
              url=link['href']
  except:
    pass  
    
url="http://www.riscosopen.org"+url
timestamp=url.split('?')[1]
archive=url.split('/')[-1]
print "export ROOL_URL=\""+url+"\""
print "export ROOL_ROM_UNIXSTAMP=\""+timestamp+"\""
print "export ROOL_ROM_ARCHIVE=\""+archive+"\""
versionmatch=re.search('\d.\d\d.zip',archive)
if versionmatch:
  version = versionmatch.group(0)[:-4]
else:
  version = "0.00"
print "export ROOL_ROM_VERSION=\""+version+"\""
if rc > 0:
  print "export ROOL_ROM_REL=\"RC"+rc+"\""
else:
  print "export ROOL_ROM_REL=\"Dev\""
