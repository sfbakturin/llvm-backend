#ifndef LLVM_LIB_TARGET_M86_M86REGISTERINFO_H
#define LLVM_LIB_TARGET_M86_M86REGISTERINFO_H

#define GET_REGINFO_HEADER
#include <M86GenRegisterInfo.inc>
#include <cstdint>
#include <llvm/ADT/BitVector.h>
#include <llvm/CodeGen/MachineBasicBlock.h>
#include <llvm/CodeGen/MachineFunction.h>
#include <llvm/CodeGen/Register.h>
#include <llvm/CodeGen/TargetFrameLowering.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/MC/MCRegister.h>

namespace llvm {

class TargetInstrInfo;
class M86Subtarget;

struct M86RegisterInfo : public M86GenRegisterInfo {
public:
  M86RegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  const std::uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                            CallingConv::ID CC) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool requiresRegisterScavenging(const MachineFunction &MF) const override;

  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  // Debug information queries.
  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // namespace llvm

#endif
