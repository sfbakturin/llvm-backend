#include <M86MCTargetDesc.h>
#include <TargetInfo/M86TargetInfo.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/TargetRegistry.h>

#define GET_REGINFO_MC_DESC
#include <M86GenRegisterInfo.inc>

#define GET_INSTRINFO_MC_DESC
#include <M86GenInstrInfo.inc>
#include <llvm/Support/Compiler.h>
#include <llvm/TargetParser/Triple.h>

#define GET_SUBTARGETINFO_MC_DESC
#include <M86GenSubtargetInfo.inc>
#include <llvm/ADT/StringRef.h>

static llvm::MCRegisterInfo *createM86MCRegisterInfo(const llvm::Triple &TT) {
  llvm::MCRegisterInfo *X = new llvm::MCRegisterInfo();
  llvm::InitM86MCRegisterInfo(X, llvm::M86::R0);
  return X;
}

static llvm::MCInstrInfo *createM86MCInstrInfo() {
  llvm::MCInstrInfo *X = new llvm::MCInstrInfo();
  llvm::InitM86MCInstrInfo(X);
  return X;
}

static llvm::MCSubtargetInfo *createM86MCSubtargetInfo(const llvm::Triple &TT,
                                                       llvm::StringRef CPU,
                                                       llvm::StringRef FS) {
  return llvm::createM86MCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

// We need to define this function for linking succeed
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeM86TargetMC() {
  llvm::Target &TheM86Target = llvm::getTheM86Target();
  // Register the MC register info.
  llvm::TargetRegistry::RegisterMCRegInfo(TheM86Target,
                                          createM86MCRegisterInfo);
  // Register the MC instruction info.
  llvm::TargetRegistry::RegisterMCInstrInfo(TheM86Target, createM86MCInstrInfo);
  // Register the MC subtarget info.
  llvm::TargetRegistry::RegisterMCSubtargetInfo(TheM86Target,
                                                createM86MCSubtargetInfo);
}
