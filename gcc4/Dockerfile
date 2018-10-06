FROM ubuntu:14.04
RUN apt-get -y update && \
 apt-get -y install libtool patch wget help2man autogen m4 gcc g++ bison flex subversion gperf sed make build-essential autoconf2.13 automake automake1.9 automake1.11 cvs doxygen dpkg-dev gettext intltool libglib2.0-dev liborbit2-dev libpopt-dev pkg-config policykit-1 realpath rman subversion unzip wget xsltproc texinfo git libx11-dev tcl
RUN \
 mkdir /home/riscos && \
 mkdir gccsdk && \
 cd gccsdk && \
 svn co svn://svn.riscos.info/gccsdk/trunk/gcc4/ gcc4
RUN \
 cd gccsdk/gcc4 && \
 ./build-world || true
RUN \
 cd gccsdk/gcc4 && \
 sed -i "s+/gccsdk+/home/riscos+g" gccsdk-params
RUN /bin/bash -c "\
 cd gccsdk/gcc4 && \
 export NUMPROC=$(grep -c ^processor /proc/cpuinfo) && \
 export MAKEFLAGS="-j${NUMPROC}" && \
 ./build-world \
 "


