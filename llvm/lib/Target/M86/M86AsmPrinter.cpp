#include <M86.h>
#include <M86AsmPrinter.h>
#include <M86InstrLowering.h>
#include <TargetInfo/M86TargetInfo.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/CodeGen/AsmPrinter.h>
#include <llvm/CodeGen/MachineFunction.h>
#include <llvm/CodeGen/MachineInstr.h>
#include <llvm/CodeGen/MachineOperand.h>
#include <llvm/CodeGen/TargetSubtargetInfo.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCInst.h>
#include <llvm/MC/MCStreamer.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/Compiler.h>
#include <llvm/Target/TargetMachine.h>
#include <memory>
#include <type_traits>

using namespace llvm;

llvm::M86AsmPrinter::M86AsmPrinter(llvm::TargetMachine &TM,
                                   std::unique_ptr<llvm::MCStreamer> Streamer)
    : llvm::AsmPrinter(TM, std::move(Streamer)), STI(TM.getMCSubtargetInfo()) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

llvm::StringRef llvm::M86AsmPrinter::getPassName() const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return "M86 Assembler Printer";
}

bool llvm::M86AsmPrinter::lowerOperand(const llvm::MachineOperand &MO,
                                       llvm::MCOperand &MCOp) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return llvm::LowerM86MachineOperandToMCOperand(MO, MCOp, *this);
}

// Simple pseudo-instructions have their lowering (with expansion to real
// instructions) auto-generated.
#include <M86GenMCPseudoLowering.inc>

void llvm::M86AsmPrinter::emitInstruction(const llvm::MachineInstr *MI) {
  M86_START_FUNCTION();

  // Do any auto-generated pseudo lowerings.
  if (emitPseudoExpansionLowering(*OutStreamer, MI)) {
    M86_END_FUNCTION();
    return;
  }

  MCInst TmpInst;
  if (!llvm::lowerM86MachineInstrToMCInst(MI, TmpInst, *this))
    EmitToStreamer(*OutStreamer, TmpInst);

  M86_END_FUNCTION();
}

bool llvm::M86AsmPrinter::runOnMachineFunction(llvm::MachineFunction &MF) {
  // Set the current MCSubtargetInfo to a copy which has the correct
  // feature bits for the current MachineFunction
  M86_START_FUNCTION();
  llvm::MCSubtargetInfo &NewSTI =
      OutStreamer->getContext().getSubtargetCopy(*TM.getMCSubtargetInfo());
  NewSTI.setFeatureBits(MF.getSubtarget().getFeatureBits());
  STI = &NewSTI;

  SetupMachineFunction(MF);
  emitFunctionBody();
  M86_END_FUNCTION();
  return false;
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeM86AsmPrinter() {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  llvm::RegisterAsmPrinter<llvm::M86AsmPrinter> X(llvm::getTheM86Target());
}
