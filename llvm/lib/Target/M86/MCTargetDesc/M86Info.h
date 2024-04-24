#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86INFO_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86INFO_H

#include <llvm/MC/MCInstrDesc.h>

namespace llvm {

namespace M86CC {

enum CondCode { EQ, GT, GE, LT, LE, NE, UEQ, UGT, UGE, ULT, ULE, UNE, INVALID };

CondCode getOppositeBranchCondition(M86CC::CondCode cond);

enum JccCondCode {
  JEQ,
  JGT,
  JGE,
  JLT,
  JLE,
  JNE,
  JUEQ,
  JUGT,
  JUGE,
  JULT,
  JULE,
  JUNE
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
