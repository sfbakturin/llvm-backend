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
  llvm::MachineFrameInfo &MFI = MF.getFrameInfo();
  auto *FI = MF.getInfo<llvm::M86MachineFunctionInfo>();
  const llvm::M86RegisterInfo *RI = STI.getRegisterInfo();
  llvm::MachineBasicBlock::iterator MBBI = MBB.begin();

  llvm::Register FPReg = llvm::M86::RF;
  llvm::Register SPReg = llvm::M86::RS;

  // Debug location must be unknown since the first debug location is used
  // to determine the end of the prologue.
  llvm::DebugLoc DL;

  std::uint64_t StackSize = alignTo(MFI.getStackSize(), getStackAlign());
  MFI.setStackSize(StackSize);

  if (!isInt<16>(StackSize)) {
    M86_END_FUNCTION();
    llvm_unreachable("Stack offs won't fit in llvm::M86::MEMLD");
  }

  // Early exit if there is no need to allocate on the stack
  if (StackSize == 0 && !MFI.adjustsStack()) {
    M86_END_FUNCTION();
    return;
  }

  // Allocate space on the stack if necessary.
  adjustReg(MBB, MBBI, DL, SPReg, SPReg, -StackSize,
            llvm::MachineInstr::FrameSetup);

  const auto &CSI = MFI.getCalleeSavedInfo();

  // The frame pointer is callee-saved, and code has been generated for us to
  // save it to the stack. We need to skip over the storing of callee-saved
  // registers as the frame pointer must be modified after it has been saved
  // to the stack, not before.
  std::advance(MBBI, CSI.size());

  if (!hasFP(MF)) {
    M86_END_FUNCTION();
    return;
  }

  // Generate new FP.
  adjustReg(MBB, MBBI, DL, FPReg, SPReg, StackSize - FI->getVarArgsSaveSize(),
            llvm::MachineInstr::FrameSetup);

  if (RI->hasStackRealignment(MF)) {
    M86_END_FUNCTION();
    llvm_unreachable("");
  }

  M86_END_FUNCTION();
}
void llvm::M86FrameLowering::emitEpilogue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {
  M86_START_FUNCTION();

  const llvm::M86RegisterInfo *RI = STI.getRegisterInfo();
  llvm::MachineFrameInfo &MFI = MF.getFrameInfo();
  auto *UFI = MF.getInfo<llvm::M86MachineFunctionInfo>();
  llvm::Register FPReg = llvm::M86::RF;
  llvm::Register SPReg = llvm::M86::RS;

  // Get the insert location for the epilogue. If there were no terminators in
  // the block, get the last instruction.
  llvm::MachineBasicBlock::iterator MBBI = MBB.end();
  llvm::DebugLoc DL;
  if (!MBB.empty()) {
    MBBI = MBB.getFirstTerminator();
    if (MBBI == MBB.end())
      MBBI = MBB.getLastNonDebugInstr();
    DL = MBBI->getDebugLoc();

    // If this is not a terminator, the actual insert location should be after
    // the last instruction.
    if (!MBBI->isTerminator())
      MBBI = std::next(MBBI);

    while (MBBI != MBB.begin() &&
           std::prev(MBBI)->getFlag(llvm::MachineInstr::FrameDestroy))
      --MBBI;
  }

  const auto &CSI = MFI.getCalleeSavedInfo();

  // Skip to before the restores of callee-saved registers.
  auto LastFrameDestroy = MBBI;
  if (!CSI.empty())
    LastFrameDestroy = std::prev(MBBI, CSI.size());

  std::uint64_t StackSize = MFI.getStackSize();
  std::uint64_t FPOffset = StackSize - UFI->getVarArgsSaveSize();

  // Restore the stack pointer using the value of the frame pointer. Only
  // necessary if the stack pointer was modified, meaning the stack size is
  // unknown.
  if (RI->hasStackRealignment(MF) || MFI.hasVarSizedObjects()) {
    llvm_unreachable("");
    assert(hasFP(MF) && "frame pointer should not have been eliminated");
    adjustReg(MBB, LastFrameDestroy, DL, SPReg, FPReg, -FPOffset,
              llvm::MachineInstr::FrameDestroy);
  }

  // Deallocate stack
  adjustReg(MBB, MBBI, DL, SPReg, SPReg, StackSize,
            llvm::MachineInstr::FrameDestroy);

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
    SavedRegs.set(llvm::M86::RA);
    SavedRegs.set(llvm::M86::RF);
  }
  // Mark BP as used if function has dedicated base pointer.
  if (hasBP(MF))
    SavedRegs.set(llvm::M86::RB);

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
                            RC, TRI, llvm::Register());
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
    TII.loadRegFromStackSlot(MBB, MI, Reg, CS.getFrameIdx(), RC, TRI,
                             llvm::Register());
    assert(MI != MBB.begin() && "loadRegFromStackSlot didn't insert any code!");
  }

  M86_END_FUNCTION();

  return true;
}

void llvm::M86FrameLowering::processFunctionBeforeFrameFinalized(
    llvm::MachineFunction &MF, llvm::RegScavenger *RS) const {
  M86_START_FUNCTION();
  llvm::MachineFrameInfo &MFI = MF.getFrameInfo();
  auto *UFI = MF.getInfo<llvm::M86MachineFunctionInfo>();
  M86_END_FUNCTION();
}

bool llvm::M86FrameLowering::hasFP(const llvm::MachineFunction &MF) const {
  M86_START_FUNCTION();

  const llvm::TargetRegisterInfo *RegInfo = MF.getSubtarget().getRegisterInfo();
  const llvm::MachineFrameInfo &MFI = MF.getFrameInfo();

  M86_END_FUNCTION();

  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         RegInfo->hasStackRealignment(MF) || MFI.hasVarSizedObjects() ||
         MFI.isFrameAddressTaken();
}

bool llvm::M86FrameLowering::hasBP(const llvm::MachineFunction &MF) const {
  M86_START_FUNCTION();
  const llvm::MachineFrameInfo &MFI = MF.getFrameInfo();
  const llvm::TargetRegisterInfo *TRI = STI.getRegisterInfo();
  M86_END_FUNCTION();
  return MFI.hasVarSizedObjects() && TRI->hasStackRealignment(MF);
}

// Eliminate ADJCALLSTACKDOWN, ADJCALLSTACKUP pseudo instructions.
llvm::MachineBasicBlock::iterator
llvm::M86FrameLowering::eliminateCallFramePseudoInstr(
    llvm::MachineFunction &MF, llvm::MachineBasicBlock &MBB,
    llvm::MachineBasicBlock::iterator MI) const {
  M86_START_FUNCTION();

  llvm::Register SPReg = llvm::M86::RS;
  llvm::DebugLoc DL = MI->getDebugLoc();

  if (!hasReservedCallFrame(MF)) {
    // If space has not been reserved for a call frame, ADJCALLSTACKDOWN and
    // ADJCALLSTACKUP must be converted to instructions manipulating the stack
    // pointer. This is necessary when there is a variable length stack
    // allocation (e.g. alloca), which means it's not possible to allocate
    // space for outgoing arguments from within the function prologue.
    std::int64_t Amount = MI->getOperand(0).getImm();

    if (Amount != 0) {
      // Ensure the stack remains aligned after adjustment.
      Amount = alignSPAdjust(Amount);

      if (MI->getOpcode() == llvm::M86::ADJCALLSTACKDOWN)
        Amount = -Amount;

      adjustReg(MBB, MI, DL, SPReg, SPReg, Amount, llvm::MachineInstr::NoFlags);
    }
  }

  M86_END_FUNCTION();

  return MBB.erase(MI);
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
    FrameReg = llvm::M86::RS;
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
// function contains variable size objects or there are vector objects
// accessed by the frame pointer. Let eliminateCallFramePseudoInstr
// preserve stack space for it.
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

void llvm::M86FrameLowering::adjustReg(llvm::MachineBasicBlock &MBB,
                                       llvm::MachineBasicBlock::iterator MBBI,
                                       const llvm::DebugLoc &DL,
                                       llvm::Register DestReg,
                                       llvm::Register SrcReg, std::int64_t Val,
                                       llvm::MachineInstr::MIFlag Flag) const {
  M86_START_FUNCTION();
  const llvm::M86InstrInfo *TII = STI.getInstrInfo();

  if (DestReg == SrcReg && Val == 0) {
    M86_END_FUNCTION();
    return;
  }

  if (isInt<16>(Val)) {
    BuildMI(MBB, MBBI, DL, TII->get(llvm::M86::ADDI), DestReg)
        .addReg(SrcReg)
        .addImm(Val)
        .setMIFlag(Flag);
  } else {
    // alloc vreg, load imm, add
    llvm_unreachable("");
  }

  M86_END_FUNCTION();
}
