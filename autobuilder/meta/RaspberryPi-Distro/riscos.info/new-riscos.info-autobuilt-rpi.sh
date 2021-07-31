#!/bin/bash

date >> /home/caliston/test
# on riscos.info's server, first build a new package index
cd /var/www/info/downloads/packages
./build-index
cd /home/caliston/RaspberryPi-Distro/riscos.info/

# then merge in the the riscpkg.org packages and blacklist those unsuitable
perl merge-distro-blacklist.pl > /var/www/info/downloads/packages/pkg/raspberrypi
