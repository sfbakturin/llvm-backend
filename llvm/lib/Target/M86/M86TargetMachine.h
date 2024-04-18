#ifndef LLVM_LIB_TARGET_M86_M86TARGETMACHINE_H
#define LLVM_LIB_TARGET_M86_M86TARGETMACHINE_H

#include <llvm/Target/TargetMachine.h>

#include <M86Subtarget.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/CodeGen/TargetPassConfig.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Target/TargetLoweringObjectFile.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Triple.h>
#include <memory>
#include <optional>

namespace llvm {

extern Target TheM86Target;

class M86TargetMachine : public LLVMTargetMachine {
public:
  M86TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   std::optional<Reloc::Model> RM,
                   std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                   bool JIT);
  ~M86TargetMachine() override;

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override;
  const M86Subtarget *getSubtargetImpl() const { return &Subtarget; }
  const M86Subtarget *getSubtargetImpl(const Function &) const override;

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  M86Subtarget Subtarget;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86TARGETMACHINE_H
