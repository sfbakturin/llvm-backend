#ifndef LLVM_LIB_TARGET_M86_M86TARGETMACHINE_H
#define LLVM_LIB_TARGET_M86_M86TARGETMACHINE_H

#include <llvm/Target/TargetMachine.h>

#include <optional>

namespace llvm {

extern Target TheM86Target;

class M86TargetMachine : public LLVMTargetMachine {
public:
  M86TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      std::optional<Reloc::Model> RM,
                      std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                      bool JIT, bool isLittle);

  M86TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      std::optional<Reloc::Model> RM,
                      std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                      bool JIT);

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86TARGETMACHINE_H
