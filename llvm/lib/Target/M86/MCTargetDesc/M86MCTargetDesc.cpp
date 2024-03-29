#include "M86MCTargetDesc.h"
#include "TargetInfo/M86TargetInfo.h"
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/TargetRegistry.h>

#define GET_REGINFO_MC_DESC
#include "M86GenRegisterInfo.inc"

static llvm::MCRegisterInfo *createM86MCRegisterInfo(const llvm::Triple &TT) {
  llvm::MCRegisterInfo *X = new llvm::MCRegisterInfo();
  llvm::InitM86MCRegisterInfo(X, llvm::M86::R0);
  return X;
}

// We need to define this function for linking succeed
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeM86TargetMC() {
  llvm::Target &TheM86Target = llvm::getTheM86Target();
  // Register the MC register info.
  llvm::TargetRegistry::RegisterMCRegInfo(TheM86Target,
                                          createM86MCRegisterInfo);
}
