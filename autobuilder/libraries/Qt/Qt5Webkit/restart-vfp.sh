# Script to restart a VFP build of Qt5Webkit without having
# to start from the beginning in the autobuilder again.
#
# Requires that $GCCSDK_INSTALL_ENV and $GCCSDK_INSTALL_CROSSBIN have been set
#
# Call from the base directory of the Qt5Webkit source tree.

if [ -z ${GCCSDK_INSTALL_ENV+x} ]; then
  echo "Set GCCSDK_INSTALL_ENV and GCCSDK_INSTALL_CROSSBIN before calling this script"
  exit 1
fi

export PKG_CONFIG=$GCCSDK_INSTALL_ENV/ro-pkg-config
export PKG_CONFIG_LIBDIR=$GCCSDK_INSTALL_ENV/lib/pkgconfig:$GCCSDK_INSTALL_ENV/share/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=$GCCSDK_INSTALL_ENV
export PATH=$GCCSDK_INSTALL_CROSSBIN:$GCCSDK_INSTALL_ENV/bin:$PATH

# $1 = base library name
install_lib() {
  cp -f lib/$1.so.1.0.0 $GCCSDK_INSTALL_ENV/vfp/lib
  ln -sf $1.so.1.0.0 $GCCSDK_INSTALL_ENV/vfp/lib/$1.so.1.0
  ln -sf $1.so.1.0.0 $GCCSDK_INSTALL_ENV/vfp/lib/$1.so.1
  ln -sf $1.so.1.0.0 $GCCSDK_INSTALL_ENV/vfp/lib/$1.so
}

( WEBKIT_OUTPUTDIR=`pwd`/build-vfp ./Tools/Scripts/build-webkit \
					  --qt --minimal \
					  --install-libs=$GCCSDK_INSTALL_ENV/vfp \
					  --install-headers=$GCCSDK_INSTALL_ENV/vfp \
					  --makeargs="-j4" \
					  --qmakearg="-spec arm-riscos-vfp-g++";
  cd build-vfp/Release;
  make install;

  # The install target does not copy these libraries to $GCCSDK_INSTALL_ENV/vfp/lib,
  # so do it manually here.
  install_lib libJavaScriptCore;
  install_lib libWebCore;
  install_lib libWebKit1;
  install_lib libWTF
)
