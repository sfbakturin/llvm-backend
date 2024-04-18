#ifndef LLVM_LIB_TARGET_M86_M86ASMPRINTER_H
#define LLVM_LIB_TARGET_M86_M86ASMPRINTER_H

#include <llvm/ADT/StringRef.h>
#include <llvm/CodeGen/AsmPrinter.h>
#include <llvm/CodeGen/MachineInstr.h>
#include <llvm/CodeGen/MachineOperand.h>
#include <llvm/MC/MCInst.h>
#include <llvm/MC/MCStreamer.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/Target/TargetMachine.h>
#include <memory>

#define DEBUG_TYPE "asm-printer"

namespace llvm {

class M86AsmPrinter : public AsmPrinter {
public:
  explicit M86AsmPrinter(TargetMachine &TM,
                         std::unique_ptr<MCStreamer> Streamer);

  StringRef getPassName() const override;

  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);

  void emitInstruction(const MachineInstr *MI) override;
  bool runOnMachineFunction(MachineFunction &MF) override;

  // Used in pseudo lowerings
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp) const;

private:
  const MCSubtargetInfo *STI;
};

} // namespace llvm

#endif
