#include <M86.h>
#include <M86InstrLowering.h>
#include <M86PassConfig.h>
#include <M86TargetMachine.h>
#include <llvm/CodeGen/TargetPassConfig.h>
#include <llvm/IR/LegacyPassManager.h>

using namespace llvm;

llvm::M86PassConfig::M86PassConfig(llvm::M86TargetMachine &TM,
                                   llvm::PassManagerBase &PM)
    : llvm::TargetPassConfig(TM, PM) {
  M86_DEBUG_FUNCTION();
}

llvm::M86TargetMachine &llvm::M86PassConfig::getM86TargetMachine() const {
  M86_DEBUG_FUNCTION();

  return getTM<llvm::M86TargetMachine>();
}

bool llvm::M86PassConfig::addInstSelector() {
  M86_DEBUG_FUNCTION();

  addPass(llvm::createM86ISelDag(getM86TargetMachine()));
  return false;
}

llvm::TargetPassConfig *
llvm::M86TargetMachine::createPassConfig(llvm::PassManagerBase &PM) {
  M86_DEBUG_FUNCTION();

  return new llvm::M86PassConfig(*this, PM);
}
