#ifndef LLVM_LIB_TARGET_M86_M86INSTRLOWERING_H
#define LLVM_LIB_TARGET_M86_M86INSTRLOWERING_H

#include <MCTargetDesc/M86MCTargetDesc.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>

namespace llvm {

class M86TargetMachine;
class FunctionPass;
class AsmPrinter;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;

bool lowerM86MachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                  AsmPrinter &AP);
bool LowerM86MachineOperandToMCOperand(const MachineOperand &MO,
                                       MCOperand &MCOp, const AsmPrinter &AP);

FunctionPass *createM86ISelDag(M86TargetMachine &TM);

} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86INSTRLOWERING_H
