#ifndef LLVM_LIB_TARGET_M86_M86PASSCONFIG_H
#define LLVM_LIB_TARGET_M86_M86PASSCONFIG_H

#include <M86TargetMachine.h>
#include <llvm/CodeGen/TargetPassConfig.h>
#include <llvm/IR/LegacyPassManager.h>

namespace llvm {

class M86PassConfig : public TargetPassConfig {
public:
  M86PassConfig(M86TargetMachine &TM, PassManagerBase &PM);

  M86TargetMachine &getM86TargetMachine() const;

  bool addInstSelector() override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86PASSCONFIG_H
