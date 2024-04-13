#include <M86.h>
#include <M86Subtarget.h>
#include <llvm/Target/TargetMachine.h>

using namespace llvm;

#define DEBUG_TYPE "M86-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include <M86FrameLowering.h>
#include <M86GenSubtargetInfo.inc>
#include <M86ISelLowering.h>
#include <M86InstrInfo.h>
#include <M86RegisterInfo.h>
#include <llvm/CodeGen/SelectionDAGTargetInfo.h>
#include <llvm/TargetParser/Triple.h>
#include <string>

llvm::M86Subtarget::M86Subtarget(const llvm::Triple &TT, const std::string &CPU,
                                 const std::string &FS,
                                 const llvm::TargetMachine &TM)
    : llvm::M86GenSubtargetInfo(TT, CPU, /*TuneCPU=*/CPU, FS), InstrInfo(),
      FrameLowering(*this), TLInfo(TM, *this) {
  M86_DEBUG_FUNCTION();
}

void llvm::M86Subtarget::anchor() { M86_DEBUG_FUNCTION(); }

const llvm::M86InstrInfo *llvm::M86Subtarget::getInstrInfo() const {
  M86_DEBUG_FUNCTION();

  return &InstrInfo;
}
const llvm::M86FrameLowering *llvm::M86Subtarget::getFrameLowering() const {
  M86_DEBUG_FUNCTION();

  return &FrameLowering;
}
const llvm::M86TargetLowering *llvm::M86Subtarget::getTargetLowering() const {
  M86_DEBUG_FUNCTION();

  return &TLInfo;
}
const llvm::M86RegisterInfo *llvm::M86Subtarget::getRegisterInfo() const {
  M86_DEBUG_FUNCTION();

  return &RegInfo;
}
const llvm::SelectionDAGTargetInfo *
llvm::M86Subtarget::getSelectionDAGInfo() const {
  M86_DEBUG_FUNCTION();

  return &TSInfo;
}
