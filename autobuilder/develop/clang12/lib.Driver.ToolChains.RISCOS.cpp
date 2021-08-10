//===--- RISCOS.cpp - RISC OS ToolChain Implementations ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RISCOS.h"
#include "Arch/ARM.h"
#include "CommonArgs.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Options.h"
#include "clang/Driver/SanitizerArgs.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Support/VirtualFileSystem.h"

using namespace clang::driver;
using namespace clang::driver::tools;
using namespace clang::driver::toolchains;
using namespace clang;
using namespace llvm::opt;

void riscos::Assembler::ConstructJob(Compilation &C, const JobAction &JA,
                                     const InputInfo &Output,
                                     const InputInfoList &Inputs,
                                     const ArgList &Args,
                                     const char *LinkingOutput) const {
  claimNoWarnArgs(Args);
  ArgStringList CmdArgs;

  // GNU as needs different flags for creating the correct output format
  // on architectures with different ABIs or optional feature sets.
  switch (getToolChain().getArch()) {
  case llvm::Triple::arm:
  case llvm::Triple::thumb: {
    StringRef MArch, MCPU;
    arm::getARMArchCPUFromArgs(Args, MArch, MCPU, /*FromAs*/ true);
    std::string Arch =
        arm::getARMTargetCPU(MCPU, MArch, getToolChain().getTriple());
    CmdArgs.push_back(Args.MakeArgString("-mcpu=" + Arch));
    break;
  }

  default:
    break;
  }

  Args.AddAllArgValues(CmdArgs, options::OPT_Wa_COMMA, options::OPT_Xassembler);

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  for (const auto &II : Inputs)
    CmdArgs.push_back(II.getFilename());

  const char *Exec = Args.MakeArgString((
		getToolChain().GetProgramPath("arm-riscos-gnueabihf-as")));
  C.addCommand(std::make_unique<Command>(JA, *this,
                                         ResponseFileSupport::AtFileCurCP(),
                                         Exec, CmdArgs, Inputs, Output));
}

void riscos::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                                  const InputInfo &Output,
                                  const InputInfoList &Inputs,
                                  const ArgList &Args,
                                  const char *LinkingOutput) const {
  const toolchains::RISCOS &ToolChain =
    static_cast<const toolchains::RISCOS &>(getToolChain());
  const Driver &D = ToolChain.getDriver();
  ArgStringList CmdArgs;

  if (!D.SysRoot.empty())
    CmdArgs.push_back(Args.MakeArgString("--sysroot=" + D.SysRoot));

  CmdArgs.push_back("--eh-frame-hdr");
  if (Args.hasArg(options::OPT_static)) {
    CmdArgs.push_back("-Bstatic");
    if (Args.hasArg(options::OPT_pie)) {
      Args.AddAllArgs(CmdArgs, options::OPT_pie);
      CmdArgs.push_back("--no-dynamic-linker");
    }
  } else {
    if (Args.hasArg(options::OPT_rdynamic))
      CmdArgs.push_back("-export-dynamic");
    if (Args.hasArg(options::OPT_shared)) {
      CmdArgs.push_back("-Bshareable");
    } else {
      Args.AddAllArgs(CmdArgs, options::OPT_pie);
      CmdArgs.push_back("-dynamic-linker");
      CmdArgs.push_back("ld-riscos/so/2");
    }
  }

  // RISCOS supports at least two ELF ABI variants.
  // Determine the correct emulation for ld.
  switch (ToolChain.getArch()) {
  case llvm::Triple::arm:
  case llvm::Triple::thumb:
    CmdArgs.push_back("-m");
    switch (ToolChain.getTriple().getEnvironment()) {
    case llvm::Triple::EABIHF:
    case llvm::Triple::GNUEABIHF:
      CmdArgs.push_back("armelf_riscos_eabi");
      break;
    // TODO: add support for GCC 4 or even AOF linking?
    default:
      CmdArgs.push_back("armelf_riscos_eabi");
      break;
    }
    break;

  default:
    break;
  }

  if (Output.isFilename()) {
    CmdArgs.push_back("-o");
    CmdArgs.push_back(Output.getFilename());
  } else {
    assert(Output.isNothing() && "Invalid output.");
  }

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nostartfiles)) {
    if (!Args.hasArg(options::OPT_shared)) {
      CmdArgs.push_back(
          Args.MakeArgString(ToolChain.GetFilePath("crt0.o")));
    }
    CmdArgs.push_back(
        Args.MakeArgString(ToolChain.GetFilePath("crti.o")));
    if (Args.hasArg(options::OPT_shared) || Args.hasArg(options::OPT_pie)) {
      CmdArgs.push_back(
          Args.MakeArgString(ToolChain.GetFilePath("crtbeginS.o")));
    } else {
      CmdArgs.push_back(
          Args.MakeArgString(ToolChain.GetFilePath("crtbegin.o")));
    }
  }

  Args.AddAllArgs(CmdArgs, options::OPT_L);
  Args.AddAllArgs(CmdArgs, options::OPT_T_Group);
  Args.AddAllArgs(CmdArgs, options::OPT_e);
  Args.AddAllArgs(CmdArgs, options::OPT_s);
  Args.AddAllArgs(CmdArgs, options::OPT_t);
  Args.AddAllArgs(CmdArgs, options::OPT_Z_Flag);
  Args.AddAllArgs(CmdArgs, options::OPT_r);

  bool NeedsSanitizerDeps = addSanitizerRuntimes(getToolChain(), Args, CmdArgs);
  bool NeedsXRayDeps = addXRayRuntime(ToolChain, Args, CmdArgs);
  AddLinkerInputs(getToolChain(), Inputs, Args, CmdArgs, JA);

  const SanitizerArgs &SanArgs = ToolChain.getSanitizerArgs();
  if (SanArgs.needsSharedRt()) {
    CmdArgs.push_back("-rpath");
    CmdArgs.push_back(Args.MakeArgString(
        ToolChain.getCompilerRTPath().c_str()));
  }

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nodefaultlibs)) {
    // Use the static OpenMP runtime with -static-openmp
    bool StaticOpenMP = Args.hasArg(options::OPT_static_openmp) &&
                        !Args.hasArg(options::OPT_static);
    addOpenMPRuntime(CmdArgs, getToolChain(), Args, StaticOpenMP);

    if (D.CCCIsCXX()) {
      if (ToolChain.ShouldLinkCXXStdlib(Args))
        ToolChain.AddCXXStdlibLibArgs(Args, CmdArgs);
    }
    if (NeedsSanitizerDeps)
      linkSanitizerRuntimeDeps(getToolChain(), CmdArgs);
    if (NeedsXRayDeps)
      linkXRayRuntimeDeps(ToolChain, CmdArgs);

    CmdArgs.push_back("-lunixlib");
  }

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nostartfiles)) {
    if (Args.hasArg(options::OPT_shared) || Args.hasArg(options::OPT_pie))
      CmdArgs.push_back(
          Args.MakeArgString(ToolChain.GetFilePath("crtendS.o")));
    else
      CmdArgs.push_back(
          Args.MakeArgString(ToolChain.GetFilePath("crtend.o")));
    CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("crtn.o")));
  }

  ToolChain.addProfileRTLibs(Args, CmdArgs);

  const char *Exec = Args.MakeArgString(
		getToolChain().GetProgramPath("arm-riscos-gnueabihf-ld"));
  C.addCommand(std::make_unique<Command>(JA, *this,
                                         ResponseFileSupport::AtFileCurCP(),
                                         Exec, CmdArgs, Inputs, Output));
}

/// RISCOS - RISCOS tool chain which can call as(1) and ld(1) directly.

RISCOS::RISCOS(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
    : Generic_ELF(D, Triple, Args) {
    // for now, assume LDFLAGS from autobuilder will find GCCSDK lib paths
}

Tool *RISCOS::buildAssembler() const {
  return new tools::riscos::Assembler(*this);
}

Tool *RISCOS::buildLinker() const { return new tools::riscos::Linker(*this); }

ToolChain::CXXStdlibType RISCOS::GetDefaultCXXStdlibType() const {
  return ToolChain::CST_Libstdcxx;
}

void RISCOS::addLibCxxIncludePaths(const llvm::opt::ArgList &DriverArgs,
                                   llvm::opt::ArgStringList &CC1Args) const {
  const std::string Candidates[] = {
    // directory relative to build tree
    getDriver().Dir + "/../include/c++/v1",
    // system install with full upstream path
    getDriver().SysRoot + "/usr/include/c++/v1",
    // system install from src
    getDriver().SysRoot + "/usr/include/c++",
  };
}

void RISCOS::addLibStdCxxIncludePaths(const llvm::opt::ArgList &DriverArgs,
                                      llvm::opt::ArgStringList &CC1Args) const {
  addLibStdCXXIncludePaths(getDriver().SysRoot, "/usr/include/g++", "", "", "",
                           "", DriverArgs, CC1Args);
}

SanitizerMask RISCOS::getSupportedSanitizers() const {
  // TODO: what sanitizers does RISC OS support?
  SanitizerMask Res = ToolChain::getSupportedSanitizers();
  return Res;
}

void RISCOS::addClangTargetOptions(const ArgList &DriverArgs,
                                   ArgStringList &CC1Args,
                                   Action::OffloadKind) const {
  const SanitizerArgs &SanArgs = getSanitizerArgs();
  if (SanArgs.hasAnySanitizer())
    CC1Args.push_back("-D_REENTRANT");
}
