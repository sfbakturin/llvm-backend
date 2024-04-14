#include <M86.h>
#include <MCTargetDesc/M86Info.h>
#include <MCTargetDesc/M86InstPrinter.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/MC/MCExpr.h>
#include <llvm/MC/MCInst.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCSymbol.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

#define DEBUG_TYPE "asm-printer"
#include <M86GenAsmWriter.inc>
#include <cassert>
#include <cstdint>
#include <llvm/ADT/StringRef.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCInstPrinter.h>
#include <llvm/MC/MCRegister.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSubtargetInfo.h>

using namespace llvm;

llvm::M86InstPrinter::M86InstPrinter(const llvm::MCAsmInfo &MAI,
                                     const llvm::MCInstrInfo &MII,
                                     const llvm::MCRegisterInfo &MRI)
    : llvm::MCInstPrinter(MAI, MII, MRI) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

void llvm::M86InstPrinter::printRegName(llvm::raw_ostream &O,
                                        llvm::MCRegister Reg) const {
  M86_START_FUNCTION();

  O << getRegisterName(Reg);

  M86_END_FUNCTION();
}

void llvm::M86InstPrinter::printInst(const llvm::MCInst *MI, uint64_t Address,
                                     llvm::StringRef Annot,
                                     const llvm::MCSubtargetInfo &STI,
                                     llvm::raw_ostream &O) {
  M86_START_FUNCTION();

  printInstruction(MI, Address, O);
  printAnnotation(O, Annot);

  M86_END_FUNCTION();
}

void llvm::M86InstPrinter::printOperand(const llvm::MCInst *MI, int OpNo,
                                        llvm::raw_ostream &O) {
  M86_START_FUNCTION();

  const llvm::MCOperand &MO = MI->getOperand(OpNo);

  if (MO.isReg()) {
    printRegName(O, MO.getReg());
    M86_END_FUNCTION();
    return;
  }

  if (MO.isImm()) {
    O << MO.getImm();
    M86_END_FUNCTION();
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI);

  M86_END_FUNCTION();
}
