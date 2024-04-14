#ifndef LLVM_LIB_TARGET_M86_M86_H
#define LLVM_LIB_TARGET_M86_M86_H

#include <MCTargetDesc/M86MCTargetDesc.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>

#if M86_DEBUG_PRINTOUT == ON
#define M86_START_FUNCTION()                                                   \
  do {                                                                         \
    llvm::errs().changeColor(llvm::raw_ostream::RED);                          \
    llvm::errs() << "[START FUNCTION]" << ' ' << __FILE__ << '(' << __LINE__   \
                 << ')' << ':' << ' ' << __func__ << "\n";                     \
    llvm::errs().changeColor(llvm::raw_ostream::WHITE);                        \
  } while (false)
#define M86_END_FUNCTION()                                                     \
  do {                                                                         \
    llvm::errs().changeColor(llvm::raw_ostream::YELLOW);                       \
    llvm::errs() << "[END FUNCTION]" << ' ' << __FILE__ << '(' << __LINE__     \
                 << ')' << ':' << ' ' << __func__ << "\n";                     \
    llvm::errs().changeColor(llvm::raw_ostream::WHITE);                        \
  } while (false)
#else
#define M86_START_FUNCTION()
#define M86_END_FUNCTION()
#endif

namespace llvm {

constexpr std::size_t M86_OPERAND_MAXIMUM_SIZE = 24;

}

#endif // LLVM_LIB_TARGET_M86_M86_H
