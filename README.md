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
# RO_GCC_CONFIG_FLAGS="--with-cpu=cortex-a72 --with-fpu=vfpv4"
```

Update for 2023: I'm going to use the Raspberry Pi OS (Raspbian) version
of Debian 11 (bullseye) and no CPU or FPU customization in order to get
a version of GCC 10.2.1 that builds by default for ARMv6 and above.
This should cover all modern RISC OS PCs, including all Raspberry Pis.

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

Please be aware that packages built with an FPU setting that includes
"neon" (ARM SIMD instructions) can be unreliable and crash with SIGEMT.
Until this is resolved, I've changed my FPU setting to "vfpv4" so that
the compiler won't generate any NEON instructions.

Note also that when you select the CPU type yourself, you must also
specify the FPU type, or else you'll get an assembler error in
`libunixlib/sys/_jmp.s` telling you the selected processor doesn't
support vstmia/vldmia in ARM mode, because VFP support isn't enabled.
This could probably be fixed by adding a directive to the .s file.

If you don't set a CPU with `RO_GCC_CONFIG_FLAGS`, the default CPU
target for GCC 10 is ARMv7-A and VFPv3, as it was before my changes.
