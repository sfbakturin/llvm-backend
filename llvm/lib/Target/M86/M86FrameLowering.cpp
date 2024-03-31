#include <M86FrameLowering.h>
#include <llvm/CodeGen/MachineBasicBlock.h>
#include <llvm/CodeGen/MachineMemOperand.h>
#include <llvm/CodeGen/TargetFrameLowering.h>
#include <llvm/Support/Alignment.h>

llvm::M86FrameLowering::M86FrameLowering()
    : llvm::TargetFrameLowering(llvm::TargetFrameLowering::StackGrowsDown,
                                llvm::Align(4), 0) {}

void llvm::M86FrameLowering::emitPrologue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {}
void llvm::M86FrameLowering::emitEpilogue(llvm::MachineFunction &MF,
                                          llvm::MachineBasicBlock &MBB) const {}

bool llvm::M86FrameLowering::hasFP(const llvm::MachineFunction &MF) const {
  return false;
}
