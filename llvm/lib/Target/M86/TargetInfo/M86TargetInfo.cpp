#include <llvm/MC/TargetRegistry.h>

#include "M86TargetInfo.h"

llvm::Target &llvm::getTheM86Target() {
  static llvm::Target TheM86Target;
  return TheM86Target;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeM86TargetInfo() {
  llvm::RegisterTarget<llvm::Triple::m86> X(llvm::getTheM86Target(), "m86", "Meliorated x86 (32-64 bit)",
                                   "M86");
}
