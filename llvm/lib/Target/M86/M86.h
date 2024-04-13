#ifndef LLVM_LIB_TARGET_M86_M86_H
#define LLVM_LIB_TARGET_M86_M86_H

#include <MCTargetDesc/M86MCTargetDesc.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>

#if M86_DEBUG_START_FUNCTION == ON
#define M86_DEBUG_FUNCTION()                                                   \
  do {                                                                         \
    llvm::errs() << __FILE__ << '(' << __LINE__ << ')' << ':' << ' '           \
                 << __func__ << "\n";                                          \
  } while (false)
#else
#define M86_DEBUG_FUNCTION()
#endif

namespace llvm {

constexpr std::size_t M86_OPERAND_MAXIMUM_SIZE = 24;

}

#endif // LLVM_LIB_TARGET_M86_M86_H
