#include <M86.h>
#include <M86TargetStreamer.h>
#include <MCTargetDesc/M86ELFMCAsmInfo.h>
#include <MCTargetDesc/M86Info.h>
#include <MCTargetDesc/M86InstPrinter.h>
#include <MCTargetDesc/M86MCTargetDesc.h>
#include <TargetInfo/M86TargetInfo.h>
#include <llvm/MC/MCDwarf.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/FormattedStream.h>

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include <M86GenRegisterInfo.inc>

#define GET_INSTRINFO_MC_DESC
#include <M86GenInstrInfo.inc>
#include <llvm/Support/Compiler.h>
#include <llvm/TargetParser/Triple.h>

#define GET_SUBTARGETINFO_MC_DESC
#include <M86GenSubtargetInfo.inc>
#include <llvm/ADT/StringRef.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCInstPrinter.h>
#include <llvm/MC/MCStreamer.h>
#include <llvm/MC/MCTargetOptions.h>

static llvm::MCRegisterInfo *createM86MCRegisterInfo(const llvm::Triple &TT) {
  M86_START_FUNCTION();

  llvm::MCRegisterInfo *X = new llvm::MCRegisterInfo();
  llvm::InitM86MCRegisterInfo(X, llvm::M86::R0);

  M86_END_FUNCTION();

  return X;
}

static llvm::MCInstrInfo *createM86MCInstrInfo() {
  M86_START_FUNCTION();

  llvm::MCInstrInfo *X = new llvm::MCInstrInfo();
  llvm::InitM86MCInstrInfo(X);

  M86_END_FUNCTION();

  return X;
}

static llvm::MCSubtargetInfo *createM86MCSubtargetInfo(const llvm::Triple &TT,
                                                       llvm::StringRef CPU,
                                                       llvm::StringRef FS) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return llvm::createM86MCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static llvm::MCAsmInfo *
createM86MCAsmInfo(const llvm::MCRegisterInfo &MRI, const llvm::Triple &TT,
                   const llvm::MCTargetOptions &Options) {
  M86_START_FUNCTION();

  llvm::MCAsmInfo *MAI = new llvm::M86ELFMCAsmInfo(TT);
  unsigned SP = MRI.getDwarfRegNum(llvm::M86::R0, true);
  llvm::MCCFIInstruction Inst =
      llvm::MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);

  M86_END_FUNCTION();

  return MAI;
}

static llvm::MCInstPrinter *
createM86MCInstPrinter(const llvm::Triple &T, unsigned SyntaxVariant,
                       const llvm::MCAsmInfo &MAI, const llvm::MCInstrInfo &MII,
                       const llvm::MCRegisterInfo &MRI) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return new llvm::M86InstPrinter(MAI, MII, MRI);
}

llvm::M86TargetStreamer::M86TargetStreamer(llvm::MCStreamer &MC)
    : llvm::MCTargetStreamer(MC) {
  M86_START_FUNCTION();
}

static llvm::MCTargetStreamer *
createM86TargetAsmStreamer(llvm::MCStreamer &S, llvm::formatted_raw_ostream &OS,
                           llvm::MCInstPrinter *InstPrint, bool isVerboseAsm) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return new llvm::M86TargetStreamer(S);
}

// We need to define this function for linking succeed
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeM86TargetMC() {
  M86_START_FUNCTION();

  llvm::Target &TheM86Target = llvm::getTheM86Target();
  // Register the MC assembler info.
  llvm::TargetRegistry::RegisterMCAsmInfo(TheM86Target, createM86MCAsmInfo);
  // Register the MC register info.
  llvm::TargetRegistry::RegisterMCRegInfo(TheM86Target,
                                          createM86MCRegisterInfo);
  // Register the MC instruction info.
  llvm::TargetRegistry::RegisterMCInstrInfo(TheM86Target, createM86MCInstrInfo);
  // Register the MC subtarget info.
  llvm::TargetRegistry::RegisterMCSubtargetInfo(TheM86Target,
                                                createM86MCSubtargetInfo);
  // Register the MC instruction printer.
  llvm::TargetRegistry::RegisterMCInstPrinter(TheM86Target,
                                              createM86MCInstPrinter);
  // Register the assembler M86 streamer.
  llvm::TargetRegistry::RegisterAsmTargetStreamer(TheM86Target,
                                                  createM86TargetAsmStreamer);
  // Register the MC code emitter.
  llvm::TargetRegistry::RegisterMCCodeEmitter(TheM86Target,
                                              createM86MCCodeEmitter);
  // Register the assembler backend.
  llvm::TargetRegistry::RegisterMCAsmBackend(TheM86Target, createM86AsmBackend);

  M86_END_FUNCTION();
}
