# riscos-gccsdk
 Mirror of the GCCSDK and autobuilder Subversion repo for RISC OS with my fixes and additions.

# Installation
 See the original [README](gcc4/README) for more information.

# My build config

Here's the `build/build-setvars` config that I'm using. I've customized the FPU
and CPU type in `autobuilder/develop/gcc/setvars`, so you'll want to change
the configure options appropriately if you're building for something
older than a Raspberry Pi 2+ or 3 (Cortex-A53).

```
RO_USE_ARMEABIHF=yes
RO_SHAREDLIBS=yes
```
