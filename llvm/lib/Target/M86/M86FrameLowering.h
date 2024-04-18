#ifndef LLVM_LIB_TARGET_M86_M86FRAMELOWERING_H
#define LLVM_LIB_TARGET_M86_M86FRAMELOWERING_H

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/BitVector.h>
#include <llvm/CodeGen/MachineBasicBlock.h>
#include <llvm/CodeGen/Register.h>
#include <llvm/CodeGen/TargetFrameLowering.h>
#include <llvm/CodeGen/TargetRegisterInfo.h>
#include <llvm/Support/TypeSize.h>

namespace llvm {

class MachineFunction;
class M86Subtarget;
class M86InstrInfo;

class M86FrameLowering : public TargetFrameLowering {
public:
  M86FrameLowering(const M86Subtarget &STI);

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;

  bool spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 ArrayRef<CalleeSavedInfo> CSI,
                                 const TargetRegisterInfo *TRI) const override;

  bool
  restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator MI,
                              MutableArrayRef<CalleeSavedInfo> CSI,
                              const TargetRegisterInfo *TRI) const override;

  void processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                           RegScavenger *RS) const override;

  /// hasFP - Return true if the specified function should have a dedicated
  /// frame pointer register. For most targets this is true only if the function
  /// has variable sized allocas or if frame pointer elimination is disabled.
  bool hasFP(const MachineFunction &MF) const override;
  bool hasBP(const MachineFunction &MF) const;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  StackOffset getFrameIndexReference(const MachineFunction &MF, int FI,
                                     Register &FrameReg) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;

private:
  void adjustStackToMatchRecords(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 bool Allocate) const;

  void adjustReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MBBI,
                 const DebugLoc &DL, Register DestReg, Register SrcReg,
                 std::int64_t Val, MachineInstr::MIFlag Flag) const;

  const M86Subtarget &STI;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86FRAMELOWERING_H
