#!/bin/bash

# on riscos.info's server, first build a new package index
pushd /var/www/info/downloads/packages
./build-index
popd

# then merge in the the riscpkg.org packages and blacklist those unsuitable
perl merge-distro-blacklist.pl > /var/www/info/downloads/packages/pkg/raspberrypi
