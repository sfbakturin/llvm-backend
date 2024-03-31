#ifndef LLVM_LIB_TARGET_M86_M86FRAMELOWERING_H
#define LLVM_LIB_TARGET_M86_M86FRAMELOWERING_H

#include <llvm/CodeGen/MachineBasicBlock.h>
#include <llvm/CodeGen/MachineMemOperand.h>
#include <llvm/CodeGen/TargetFrameLowering.h>

namespace llvm {

class M86FrameLowering : public TargetFrameLowering {
public:
  M86FrameLowering();

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  /// hasFP - Return true if the specified function should have a dedicated
  /// frame pointer register. For most targets this is true only if the function
  /// has variable sized allocas or if frame pointer elimination is disabled.
  bool hasFP(const MachineFunction &MF) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86FRAMELOWERING_H
