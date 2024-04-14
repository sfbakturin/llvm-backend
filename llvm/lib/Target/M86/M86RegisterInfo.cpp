#include <M86.h>
#include <M86InstrInfo.h>
#include <M86RegisterInfo.h>
#include <M86Subtarget.h>
#include <MCTargetDesc/M86MCTargetDesc.h>
#include <llvm/ADT/BitVector.h>
#include <llvm/CodeGen/MachineFrameInfo.h>
#include <llvm/CodeGen/MachineFunction.h>
#include <llvm/CodeGen/MachineInstrBuilder.h>
#include <llvm/CodeGen/MachineModuleInfo.h>
#include <llvm/CodeGen/MachineRegisterInfo.h>
#include <llvm/CodeGen/RegisterScavenging.h>
#include <llvm/CodeGen/TargetFrameLowering.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

using namespace llvm;

#define DEBUG_TYPE "M86-reg-info"

#define GET_REGINFO_TARGET_DESC
#include <M86FrameLowering.h>
#include <M86GenRegisterInfo.inc>
#include <cassert>
#include <cstdint>
#include <llvm/CodeGen/MachineBasicBlock.h>
#include <llvm/CodeGen/MachineInstr.h>
#include <llvm/CodeGen/Register.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/DebugLoc.h>
#include <llvm/MC/MCRegister.h>
#include <llvm/Support/MathExtras.h>

llvm::M86RegisterInfo::M86RegisterInfo()
    : llvm::M86GenRegisterInfo(llvm::M86::R0) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

const llvm::MCPhysReg *llvm::M86RegisterInfo::getCalleeSavedRegs(
    const llvm::MachineFunction *MF) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return llvm::CSR_M86_SaveList;
}

llvm::BitVector
llvm::M86RegisterInfo::getReservedRegs(const llvm::MachineFunction &MF) const {
  M86_START_FUNCTION();

  const llvm::M86FrameLowering *TFI = getFrameLowering(MF);

  BitVector Reserved(getNumRegs());
  Reserved.set(llvm::M86::R1);

  if (TFI->hasFP(MF)) {
    Reserved.set(llvm::M86::R2);
  }

  M86_END_FUNCTION();

  return Reserved;
}

bool llvm::M86RegisterInfo::requiresRegisterScavenging(
    const llvm::MachineFunction &MF) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return false;
}

bool llvm::M86RegisterInfo::eliminateFrameIndex(
    llvm::MachineBasicBlock::iterator II, int SPAdj, unsigned FIOperandNum,
    llvm::RegScavenger *RS) const {
  M86_START_FUNCTION();

  assert(SPAdj == 0 && "Unexpected non-zero SPAdj value");

  llvm::MachineInstr &MI = *II;
  llvm::MachineFunction &MF = *MI.getParent()->getParent();
  llvm::DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  llvm::Register FrameReg;
  int Offset = getFrameLowering(MF)
                   ->getFrameIndexReference(MF, FrameIndex, FrameReg)
                   .getFixed();
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (!isInt<16>(Offset)) {
    llvm_unreachable("");
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false, false, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);

  M86_END_FUNCTION();

  return false;
}

llvm::Register
llvm::M86RegisterInfo::getFrameRegister(const llvm::MachineFunction &MF) const {
  M86_START_FUNCTION();

  const llvm::TargetFrameLowering *TFI = getFrameLowering(MF);

  M86_END_FUNCTION();

  return TFI->hasFP(MF) ? llvm::M86::R2 : llvm::M86::R1;
}

const std::uint32_t *
llvm::M86RegisterInfo::getCallPreservedMask(const llvm::MachineFunction &MF,
                                            llvm::CallingConv::ID CC) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return llvm::CSR_M86_RegMask;
}
