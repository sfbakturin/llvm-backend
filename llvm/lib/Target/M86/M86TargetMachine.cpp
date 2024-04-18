#include <llvm/CodeGen/TargetLoweringObjectFileImpl.h>
#include <llvm/MC/TargetRegistry.h>
#include <optional>

#include <M86.h>
#include <M86Subtarget.h>
#include <M86TargetMachine.h>
#include <TargetInfo/M86TargetInfo.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/Compiler.h>
#include <llvm/Target/TargetLoweringObjectFile.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Triple.h>
#include <memory>
#include <string>

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeM86Target() {
  M86_START_FUNCTION();

  // Register the target.
  llvm::RegisterTargetMachine<llvm::M86TargetMachine> A(
      llvm::getTheM86Target());

  M86_END_FUNCTION();
}

static llvm::Reloc::Model
getEffectiveRelocModel(bool JIT, std::optional<llvm::Reloc::Model> RM) {
  M86_START_FUNCTION();

  if (!RM || JIT) {
    M86_END_FUNCTION();
    return llvm::Reloc::Static;
  }

  M86_END_FUNCTION();

  return *RM;
}

llvm::M86TargetMachine::M86TargetMachine(
    const llvm::Target &T, const llvm::Triple &TT, llvm::StringRef CPU,
    llvm::StringRef FS, const llvm::TargetOptions &Options,
    std::optional<llvm::Reloc::Model> RM,
    std::optional<llvm::CodeModel::Model> CM, llvm::CodeGenOptLevel OL,
    bool JIT)
    : llvm::LLVMTargetMachine(
          T, "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32", TT, CPU, FS, Options,
          llvm::Reloc::Static,
          llvm::getEffectiveCodeModel(CM, llvm::CodeModel::Small), OL),
      TLOF(std::make_unique<llvm::TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  M86_START_FUNCTION();
  initAsmInfo();
  M86_END_FUNCTION();
}

llvm::M86TargetMachine::~M86TargetMachine() = default;

llvm::TargetLoweringObjectFile *
llvm::M86TargetMachine::getObjFileLowering() const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return TLOF.get();
}

const llvm::M86Subtarget *
llvm::M86TargetMachine::getSubtargetImpl(const llvm::Function &) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return &Subtarget;
}
