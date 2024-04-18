#include <llvm/MC/TargetRegistry.h>

#include <M86.h>
#include <M86TargetInfo.h>
#include <llvm/Support/Compiler.h>
#include <llvm/TargetParser/Triple.h>

using namespace llvm;

llvm::Target &llvm::getTheM86Target() {
  M86_START_FUNCTION();
  static llvm::Target TheM86Target;
  M86_END_FUNCTION();
  return TheM86Target;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeM86TargetInfo() {
  M86_START_FUNCTION();
  llvm::RegisterTarget<llvm::Triple::m86> X(llvm::getTheM86Target(), "m86",
                                            "Meliorated x86 (32 bit)", "M86");
  M86_END_FUNCTION();
}
