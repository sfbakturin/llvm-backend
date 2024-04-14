#include <M86.h>
#include <M86FrameLowering.h>
#include <M86GenRegisterInfo.inc>
#include <M86MachineFunctionInfo.h>
#include <M86RegisterInfo.h>
#include <M86Subtarget.h>
#include <cassert>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/BitVector.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/CodeGen/MachineBasicBlock.h>
#include <llvm/CodeGen/MachineFrameInfo.h>
#include <llvm/CodeGen/MachineFunction.h>
#include <llvm/CodeGen/MachineInstrBuilder.h>
#include <llvm/CodeGen/MachineMemOperand.h>
#include <llvm/CodeGen/MachineModuleInfo.h>
#include <llvm/CodeGen/Register.h>
#include <llvm/CodeGen/RegisterScavenging.h>
#include <llvm/CodeGen/TargetFrameLowering.h>
#include <llvm/CodeGen/TargetInstrInfo.h>
#include <llvm/CodeGen/TargetRegisterInfo.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/Alignment.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/TypeSize.h>

using namespace llvm;

#define DEBUG_TYPE "M86-frame-lowering"

llvm::M86FrameLowering::M86FrameLowering(const llvm::M86Subtarget &STI)
    : llvm::TargetFrameLowering(llvm::TargetFrameLowering::StackGrowsDown,
                                llvm::Align(4), 0),
      STI(STI) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

void llvm::M86FrameLowering::emitPrologue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}
void llvm::M86FrameLowering::emitEpilogue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

void llvm::M86FrameLowering::determineCalleeSaves(
    llvm::MachineFunction &MF, llvm::BitVector &SavedRegs,
    llvm::RegScavenger *RS) const {
  M86_START_FUNCTION();

  llvm::TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  // Unconditionally spill RA and FP only if the function uses a frame
  // pointer.
  if (hasFP(MF)) {
    SavedRegs.set(llvm::M86::R0);
    SavedRegs.set(llvm::M86::R2);
  }

  M86_END_FUNCTION();
}

bool llvm::M86FrameLowering::spillCalleeSavedRegisters(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock::iterator MI,
    llvm::ArrayRef<llvm::CalleeSavedInfo> CSI,
    const llvm::TargetRegisterInfo *TRI) const {
  M86_START_FUNCTION();

  if (CSI.empty()) {
    M86_END_FUNCTION();
    return true;
  }

  llvm::MachineFunction *MF = MBB.getParent();
  const llvm::TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();

  for (auto &CS : CSI) {
    // Insert the spill to the stack frame.
    llvm::Register Reg = CS.getReg();
    const llvm::TargetRegisterClass *RC = TRI->getMinimalPhysRegClass(Reg);
    TII.storeRegToStackSlot(MBB, MI, Reg, !MBB.isLiveIn(Reg), CS.getFrameIdx(),
                            RC, TRI, 0);
  }

  M86_END_FUNCTION();

  return true;
}

bool llvm::M86FrameLowering::restoreCalleeSavedRegisters(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock::iterator MI,
    llvm::MutableArrayRef<llvm::CalleeSavedInfo> CSI,
    const llvm::TargetRegisterInfo *TRI) const {
  M86_START_FUNCTION();

  if (CSI.empty()) {
    M86_END_FUNCTION();
    return true;
  }

  llvm::MachineFunction *MF = MBB.getParent();
  const llvm::TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();

  // Insert in reverse order.
  // loadRegFromStackSlot can insert multiple instructions.
  for (auto &CS : llvm::reverse(CSI)) {
    llvm::Register Reg = CS.getReg();
    const llvm::TargetRegisterClass *RC = TRI->getMinimalPhysRegClass(Reg);
    TII.loadRegFromStackSlot(MBB, MI, Reg, CS.getFrameIdx(), RC, TRI, 0);
    assert(MI != MBB.begin() && "loadRegFromStackSlot didn't insert any code!");
  }

  M86_END_FUNCTION();

  return true;
}

bool llvm::M86FrameLowering::hasFP(const llvm::MachineFunction &MF) const {
  M86_START_FUNCTION();

  const llvm::TargetRegisterInfo *RegInfo = MF.getSubtarget().getRegisterInfo();

  const llvm::MachineFrameInfo &MFI = MF.getFrameInfo();

  M86_END_FUNCTION();

  return MF.getTarget().Options.DisableFramePointerElim(
             MF) || // -fomit-frame-pointer
         RegInfo->hasStackRealignment(MF) ||
         MFI.hasVarSizedObjects() || MFI.isFrameAddressTaken();
}

llvm::StackOffset llvm::M86FrameLowering::getFrameIndexReference(
    const llvm::MachineFunction &MF, int FI, llvm::Register &FrameReg) const {
  M86_START_FUNCTION();

  const llvm::MachineFrameInfo &MFI = MF.getFrameInfo();
  const llvm::TargetRegisterInfo *RI = MF.getSubtarget().getRegisterInfo();
  const auto *UFI = MF.getInfo<llvm::M86MachineFunctionInfo>();

  // Callee-saved registers should be referenced relative to the stack
  // pointer (positive offset), otherwise use the frame pointer (negative
  // offset).
  const auto &CSI = MFI.getCalleeSavedInfo();
  int MinCSFI = 0;
  int MaxCSFI = -1;
  int Offset;

  Offset = MFI.getObjectOffset(FI) - getOffsetOfLocalArea() +
           MFI.getOffsetAdjustment();

  if (CSI.size()) {
    MinCSFI = CSI[0].getFrameIdx();
    MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
  }

  if (FI >= MinCSFI && FI <= MaxCSFI) {
    FrameReg = llvm::M86::R1;
    Offset += MFI.getStackSize();
  } else if (RI->hasStackRealignment(MF) && !MFI.isFixedObjectIndex(FI)) {
    llvm_unreachable("");
  } else {
    FrameReg = RI->getFrameRegister(MF);
    if (hasFP(MF)) {
      Offset += UFI->getVarArgsSaveSize();
    } else {
      Offset += MFI.getStackSize();
    }
  }

  M86_END_FUNCTION();

  return llvm::StackOffset::getFixed(Offset);
}

// Not preserve stack space within prologue for outgoing variables when the
// function contains variable size objects or there are vector objects accessed
// by the frame pointer.
// Let eliminateCallFramePseudoInstr preserve stack space for it.
bool llvm::M86FrameLowering::hasReservedCallFrame(
    const llvm::MachineFunction &MF) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return !MF.getFrameInfo().hasVarSizedObjects();
}

void llvm::M86FrameLowering::adjustStackToMatchRecords(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock::iterator MBBI,
    bool Allocate) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  llvm_unreachable("");
}
