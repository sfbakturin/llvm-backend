#include <llvm/CodeGen/Passes.h>
#include <llvm/CodeGen/TargetLoweringObjectFileImpl.h>
#include <llvm/CodeGen/TargetPassConfig.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Transforms/Scalar.h>
#include <optional>

#include "M86TargetMachine.h"
#include "TargetInfo/M86TargetInfo.h"

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeM86Target() {
  // Register the target.
  llvm::RegisterTargetMachine<llvm::M86TargetMachine> A(llvm::getTheM86Target());
}

static std::string computeDataLayout(const llvm::Triple &TT, llvm::StringRef CPU,
                                     const llvm::TargetOptions &Options,
                                     bool IsLittle) {
  std::string Ret = "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32";
  return Ret;
}

static llvm::Reloc::Model getEffectiveRelocModel(bool JIT,
                                           std::optional<llvm::Reloc::Model> RM) {
  if (!RM || JIT)
     return llvm::Reloc::Static;
  return *RM;
}

llvm::M86TargetMachine::M86TargetMachine(const llvm::Target &T, const llvm::Triple &TT,
                                         llvm::StringRef CPU, llvm::StringRef FS,
                                         const llvm::TargetOptions &Options,
                                         std::optional<llvm::Reloc::Model> RM,
                                         std::optional<llvm::CodeModel::Model> CM,
                                         llvm::CodeGenOpt::Level OL, bool JIT,
                                         bool IsLittle)
    : llvm::LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options, IsLittle), TT,
                        CPU, FS, Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<llvm::TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}

llvm::M86TargetMachine::M86TargetMachine(const llvm::Target &T, const llvm::Triple &TT,
                                         llvm::StringRef CPU, llvm::StringRef FS,
                                         const llvm::TargetOptions &Options,
                                         std::optional<llvm::Reloc::Model> RM,
                                         std::optional<llvm::CodeModel::Model> CM,
                                         llvm::CodeGenOpt::Level OL, bool JIT)
    : llvm::M86TargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, true) {}

llvm::TargetPassConfig *llvm::M86TargetMachine::createPassConfig(llvm::PassManagerBase &PM) {
  return new llvm::TargetPassConfig(*this, PM);
}
