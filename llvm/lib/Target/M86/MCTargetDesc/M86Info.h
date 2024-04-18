#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86INFO_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86INFO_H

#include <llvm/MC/MCInstrDesc.h>

namespace llvm {

namespace M86CC {

enum CondCode { EQ, NE, LE, GT, LEU, GTU, INVALID };

CondCode getOppositeBranchCondition(M86CC::CondCode cond);

enum JccCondCode {
  JEQ = 0x1,
  JNE = 0x2,
  JLE = 0x3,
  JGT = 0x4,
  JLEU = 0x5,
  JGTU = 0x6
};

} // namespace M86CC

namespace M86Op {

enum OperandType : unsigned {
  OPERAND_UNSIGNED_IMM = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_SIGNED_IMM
};

} // namespace M86Op

} // end namespace llvm

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86INFO_H
