# riscos-gccsdk
 Mirror of the GCCSDK and autobuilder Subversion repo for RISC OS with my fixes and additions.

# Installation
 See the original [README](gcc4/README) for more information.

# My build config

Here's the `build/build-setvars` config that I'm using. To install GCC 10,
run `../autobuilder/build gcc` from the build directory you created with
your `build-setvars` file.

```
RO_USE_ARMEABIHF=yes
RO_SHAREDLIBS=yes
RO_GCC_CONFIG_FLAGS="--with-cpu=cortex-a72 --with-fpu=neon-vfpv4"
```

I originally patched GCC 10's `setvars` file to optimize for Cortex-A53,
but then realized that it should be possible to change the CPU and FPU
type by editing `build-setvars`. Whichever `--with-cpu=` and `--with-fpu=`
settings you specify when configuring GCC will become the default for
programs compiled with that compiler, unless you override with `-mcpu=`
or `-mfpu=`.

The reason I want to customize the GCC toolchain configure flags to
specify a particular CPU and FPU is that it makes it easier for me to
build all of the packages optimized for that CPU without having to edit
all the autobuilder scripts to add `-mcpu=` and `-mfpu=` flags to them.
Instead, I build GCC 10 to default to the CPU and FPU to optimize for,
and then build all the packages using that toolchain.

If you don't set a CPU in `RO_GCC_CONFIG_FLAGS` in `build-setvars`, the
default target for GCC 10 is ARMv7-A and VFPv3, as it was before I added
the ability to customize the CPU type. Please note that when you select
the CPU type yourself, you must also select the FPU type, or else you'll
get an assembler error in `libunixlib/sys/_jmp.s` telling you the selected
processor doesn't support vstmia/vldmia in ARM mode, even though it does.
Perhaps that `.s` file should be explicitly enabling VFP instructions?

Specifying the FPU along with the CPU when configuring GCC causes it to
add `-mfpu=` to the assembler flags, which prevents this error. There may
be new bugs introduced by changing binutils versions, so I'm not going to
spend any more time trying to upgrade binutils in the hope that a newer
version will automatically recognize Cortex-A CPUs as supporting VFPv4.
I tested binutils 2.32 and it behaves the same as 2.30, 2.31, and 2.31.1,
so I'm going to stay with binutils 2.31.1, as it looks fairly stable.
