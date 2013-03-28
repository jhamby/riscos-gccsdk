#!/usr/bin/python
# horrifically insecure

from BeautifulSoup import BeautifulSoup
import urllib2
import os
url="http://www.riscosopen.org/content/downloads/other-zipfiles"
page=urllib2.urlopen(url)
soup = BeautifulSoup(page.read())
devel = os.getenv('ROOL_ROM_DEV')
if (devel):
  filename = "BCM2835Dev.5.19.zip"
else:
  filename = "BCM2835.5.19.zip"
#print soup
for link in soup.findAll('a'): # find all links
  if filename in link['href']:
    url=link['href']
    
url="http://www.riscosopen.org"+url
timestamp=url.split('?')[1]
archive=url.split('/')[-1]
print "export ROOL_URL=\""+url+"\""
print "export ROOL_ROM_UNIXSTAMP=\""+timestamp+"\""
print "export ROOL_ROM_ARCHIVE=\""+archive+"\""
