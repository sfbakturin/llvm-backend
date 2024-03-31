#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86INFO_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86INFO_H

#include <llvm/MC/MCInstrDesc.h>

namespace llvm {

namespace M86Op {
enum OperandType : unsigned {
  OPERAND_M86IMM = MCOI::OPERAND_FIRST_TARGET,
};
} // namespace M86Op

} // end namespace llvm

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86INFO_H
