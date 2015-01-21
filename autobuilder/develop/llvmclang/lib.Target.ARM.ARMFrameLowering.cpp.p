--- lib/Target/ARM/ARMFrameLowering.cpp.orig	2014-06-26 20:29:59.000000000 +0100
+++ lib/Target/ARM/ARMFrameLowering.cpp	2015-01-18 20:09:36.512288833 +0000
@@ -54,6 +54,12 @@
     return true;
 
   const MachineFrameInfo *MFI = MF.getFrameInfo();
+  
+  if (STI.isTargetRISCOS()) {
+    if (MFI->estimateStackSize(MF) >= 256 || MFI->hasCalls())
+      return true;
+  }
+
   // Always eliminate non-leaf frame pointers.
   return ((MF.getTarget().Options.DisableFramePointerElim(MF) &&
            MFI->hasCalls()) ||
@@ -189,6 +195,13 @@
   if (MF.getFunction()->getCallingConv() == CallingConv::GHC)
     return;
 
+  if (STI.isTargetRISCOS() && hasFP(MF)) {
+    // mov ip, sp
+    AddDefaultCC(AddDefaultPred(BuildMI(MBB, MBBI, dl, TII.get(ARM::MOVr), ARM::R12)
+                   .addReg(ARM::SP)
+                   .setMIFlags(MachineInstr::FrameSetup)));
+  }
+
   // Allocate the vararg register save area.
   if (ArgRegsSaveSize) {
     emitSPUpdate(isARM, MBB, MBBI, dl, TII, -ArgRegsSaveSize,
@@ -219,6 +232,14 @@
     unsigned Reg = CSI[i].getReg();
     int FI = CSI[i].getFrameIdx();
     switch (Reg) {
+    case ARM::PC:
+    case ARM::SP:
+      // RISCOS: We actually substitute R12 for SP at function entry,
+      // but at this point llvm sees it as SP.
+      if (STI.isTargetRISCOS()) {
+        GPRCS1Size += 4;
+        break;
+      }
     case ARM::R8:
     case ARM::R9:
     case ARM::R10:
@@ -265,10 +286,18 @@
   unsigned GPRCS1Offset = GPRCS2Offset + GPRCS2Size;
   int FramePtrOffsetInPush = 0;
   if (HasFP) {
-    FramePtrOffsetInPush = MFI->getObjectOffset(FramePtrSpillFI)
+    if (STI.isTargetRISCOS()) {
+      FramePtrOffsetInPush = -(4 + ArgRegsSaveSize);
+      // I believe the +12 below represents the last three registers saved
+      // in the prologue (r12,r14,pc) after the frame pointer (r11).
+      AFI->setFramePtrSpillOffset(MFI->getObjectOffset(FramePtrSpillFI) +
+				  NumBytes + 12);
+    } else {
+      FramePtrOffsetInPush = MFI->getObjectOffset(FramePtrSpillFI)
                            + GPRCS1Size + ArgRegsSaveSize;
-    AFI->setFramePtrSpillOffset(MFI->getObjectOffset(FramePtrSpillFI) +
-                                NumBytes);
+      AFI->setFramePtrSpillOffset(MFI->getObjectOffset(FramePtrSpillFI) +
+				  NumBytes);
+    }
   }
   AFI->setGPRCalleeSavedArea1Offset(GPRCS1Offset);
   AFI->setGPRCalleeSavedArea2Offset(GPRCS2Offset);
@@ -295,8 +324,9 @@
     //
     // Adjust NumBytes to represent the stack slots below the DPRCS2 area.
     NumBytes += MFI->getObjectOffset(D8SpillFI);
-  } else
+  } else {
     NumBytes = DPRCSOffset;
+  }
 
   if (STI.isTargetWindows() && WindowsRequiresStackProbe(MF, NumBytes)) {
     uint32_t NumWords = NumBytes >> 2;
@@ -342,6 +372,39 @@
                                 .setMIFlags(MachineInstr::FrameSetup)));
     NumBytes = 0;
   }
+  
+  // Do we need a stack check?
+  if (AFI->hasStackExtension()) {
+    const char *stkovf_split_func;
+    bool splitBig = (NumBytes >= 256);
+
+    // Marking the stack extension instructions as part of FrameSetup
+    // causes problems in ARMAsmPrinter::EmitUnwindingInstruction.
+    // (We've already had to make allowances there for the sub r11, r12, #n
+    // instruction).
+    // So we don't mark them as such and hope that the compiler doesn't
+    // move them somewhere after a stack access/function call.
+    if (splitBig) {
+      stkovf_split_func = "__rt_stkovf_split_big";
+      emitRegPlusImmediate(/*isARM = */true, MBB, MBBI, dl, TII,
+			   ARM::R12, ARM::SP, -NumBytes/*, MachineInstr::FrameSetup*/);
+      AddDefaultPred(BuildMI(MBB, MBBI, dl, TII.get(ARM::CMPrr))
+                       .addReg(ARM::R12)
+		       .addReg(ARM::R10)/*
+                       .setMIFlags(MachineInstr::FrameSetup)*/);
+    } else {
+      stkovf_split_func = "__rt_stkovf_split_small";
+      AddDefaultPred(BuildMI(MBB, MBBI, dl, TII.get(ARM::CMPrr))
+                       .addReg(ARM::SP)
+		       .addReg(ARM::R10)/*
+                       .setMIFlags(MachineInstr::FrameSetup)*/);
+    }
+
+    BuildMI(MBB, MBBI, dl, TII.get(ARM::BL_pred))
+        .addExternalSymbol(stkovf_split_func)
+	.addImm((unsigned)ARMCC::LT).addReg(0)/*
+        .setMIFlags(MachineInstr::FrameSetup)*/;
+  }
 
   unsigned adjustedGPRCS1Size = GPRCS1Size;
   if (NumBytes) {
@@ -413,8 +476,8 @@
   // that push.
   if (HasFP) {
     emitRegPlusImmediate(!AFI->isThumbFunction(), MBB, GPRCS1Push, dl, TII,
-                         FramePtr, ARM::SP, FramePtrOffsetInPush,
-                         MachineInstr::FrameSetup);
+                         FramePtr, (STI.isTargetRISCOS() && AFI->hasStackFrame()) ? ARM::R12 : ARM::SP,
+                         FramePtrOffsetInPush, MachineInstr::FrameSetup);
     if (FramePtrOffsetInPush) {
       CFAOffset += FramePtrOffsetInPush;
       unsigned CFIIndex = MMI.addFrameInst(MCCFIInstruction::createDefCfa(
@@ -513,7 +576,8 @@
   // sure if we also have VLAs, we have a base pointer for frame access.
   // If aligned NEON registers were spilled, the stack has already been
   // realigned.
-  if (!AFI->getNumAlignedDPRCS2Regs() && RegInfo->needsStackRealignment(MF)) {
+  if (!AFI->getNumAlignedDPRCS2Regs() && RegInfo->needsStackRealignment(MF) &&
+      !STI.isTargetRISCOS()) {
     unsigned MaxAlign = MFI->getMaxAlignment();
     assert (!AFI->isThumb1OnlyFunction());
     if (!AFI->isThumbFunction()) {
@@ -613,7 +677,7 @@
 
     // Reset SP based on frame pointer only if the stack frame extends beyond
     // frame pointer stack slot or target is ELF and the function has FP.
-    if (AFI->shouldRestoreSPFromFP()) {
+    if (AFI->shouldRestoreSPFromFP() && !STI.isTargetRISCOS()) {
       NumBytes = AFI->getFramePtrSpillOffset() - NumBytes;
       if (NumBytes) {
         if (isARM)
@@ -645,7 +709,7 @@
                                  ARM::SP)
             .addReg(FramePtr));
       }
-    } else if (NumBytes &&
+    } else if (NumBytes && (!STI.isTargetRISCOS() || (STI.isTargetRISCOS() && !hasFP(MF))) &&
                !tryFoldSPUpdateIntoPushPop(STI, MF, MBBI, NumBytes))
         emitSPUpdate(isARM, MBB, MBBI, dl, TII, NumBytes);
 
@@ -684,9 +748,14 @@
       // Add the default predicate in Thumb mode.
       if (STI.isThumb()) MIB.addImm(ARMCC::AL).addReg(0);
     } else if (RetOpcode == ARM::TCRETURNri) {
-      BuildMI(MBB, MBBI, dl,
-              TII.get(STI.isThumb() ? ARM::tTAILJMPr : ARM::TAILJMPr)).
-        addReg(JumpTarget.getReg(), RegState::Kill);
+      if (STI.isTargetRISCOS() && !STI.hasV4TOps()) {
+        BuildMI(MBB, MBBI, dl, TII.get(ARM::MOVPCRX)).
+          addReg(JumpTarget.getReg(), RegState::Kill);
+      } else {
+        BuildMI(MBB, MBBI, dl,
+                TII.get(STI.isThumb() ? ARM::tTAILJMPr : ARM::TAILJMPr)).
+          addReg(JumpTarget.getReg(), RegState::Kill);
+      }
     }
 
     MachineInstr *NewMI = std::prev(MBBI);
@@ -698,7 +767,7 @@
     MBBI = NewMI;
   }
 
-  if (ArgRegsSaveSize)
+  if (ArgRegsSaveSize && !STI.isTargetRISCOS())
     emitSPUpdate(isARM, MBB, MBBI, dl, TII, ArgRegsSaveSize);
 }
 
@@ -776,8 +845,10 @@
         FrameReg = RegInfo->getFrameRegister(MF);
         return FPOffset;
       }
-    } else if (Offset > (FPOffset < 0 ? -FPOffset : FPOffset)) {
+    } else if (Offset > (FPOffset < 0 ? -FPOffset : FPOffset) && !STI.isTargetRISCOS()) {
       // Otherwise, use SP or FP, whichever is closer to the stack slot.
+      // RISCOS: The stack may have been extended, so we always use SP as FP could be
+      // in a different stack chunk.
       FrameReg = RegInfo->getFrameRegister(MF);
       return FPOffset;
     }
@@ -849,8 +920,12 @@
       MachineInstrBuilder MIB =
         AddDefaultPred(BuildMI(MBB, MI, DL, TII.get(StmOpc), ARM::SP)
                        .addReg(ARM::SP).setMIFlags(MIFlags));
-      for (unsigned i = 0, e = Regs.size(); i < e; ++i)
-        MIB.addReg(Regs[i].first, getKillRegState(Regs[i].second));
+      for (unsigned i = 0, e = Regs.size(); i < e; ++i) {
+        if (Regs[i].first == ARM::SP)
+          MIB.addReg(ARM::R12, getKillRegState(Regs[i].second));
+        else
+          MIB.addReg(Regs[i].first, getKillRegState(Regs[i].second));
+      }
     } else if (Regs.size() == 1) {
       MachineInstrBuilder MIB = BuildMI(MBB, MI, DL, TII.get(StrOpc),
                                         ARM::SP)
@@ -919,11 +994,27 @@
     if (Regs.empty())
       continue;
     if (Regs.size() > 1 || LdrOpc == 0) {
-      MachineInstrBuilder MIB =
-        AddDefaultPred(BuildMI(MBB, MI, DL, TII.get(LdmOpc), ARM::SP)
+      MachineInstrBuilder MIB;
+        if (STI.isTargetRISCOS() && AFI->hasStackFrame() && hasFP(MF)) {
+          MIB = AddDefaultPred(BuildMI(MBB, MI, DL, TII.get(ARM::LDMDB), ARM::R11));
+        } else {
+          MIB = AddDefaultPred(BuildMI(MBB, MI, DL, TII.get(LdmOpc), ARM::SP)
                        .addReg(ARM::SP));
-      for (unsigned i = 0, e = Regs.size(); i < e; ++i)
-        MIB.addReg(Regs[i], getDefRegState(true));
+        }
+      bool seenLRPC = false;
+      for (unsigned i = 0, e = Regs.size(); i < e; ++i) {
+        if (STI.isTargetRISCOS() && AFI->hasStackFrame() && hasFP(MF) && !isInterrupt) {
+          if (Regs[i] == ARM::LR || Regs[i] == ARM::PC) {
+            // Restore one or the other, but not both.
+            if (!seenLRPC)
+              MIB.addReg(Regs[i], getDefRegState(true));
+              seenLRPC = true;
+          } else  
+            MIB.addReg(Regs[i], getDefRegState(true));
+        }
+        else
+          MIB.addReg(Regs[i], getDefRegState(true));
+      }
       if (DeleteRet) {
         MIB.copyImplicitOps(&*MI);
         MI->eraseFromParent();
@@ -1393,6 +1484,20 @@
   MachineRegisterInfo &MRI = MF.getRegInfo();
   unsigned FramePtr = RegInfo->getFrameRegister(MF);
 
+  if (STI.isTargetRISCOS() && STI.APCSStackCheck()) {
+    // A stack check is needed if more than 256 bytes of stack is required
+    // or the function requires a frame pointer (which implies that it calls
+    // other functions).
+    if (MFI->estimateStackSize(MF) >= 256 || hasFP(MF)) {
+      AFI->setHasStackExtension(true);
+      CanEliminateFrame = false;
+      MRI.setPhysRegUsed(ARM::R11);
+      MRI.setPhysRegUsed(ARM::SP);
+      MRI.setPhysRegUsed(ARM::LR);
+      MRI.setPhysRegUsed(ARM::PC);
+    }
+  }
+
   // Spill R4 if Thumb2 function requires stack realignment - it will be used as
   // scratch register. Also spill R4 if Thumb2 function has varsized objects,
   // since it's not always possible to restore sp from fp in a single
