#ifndef LLVM_LIB_TARGET_M86_M86_H
#define LLVM_LIB_TARGET_M86_M86_H

#include <MCTargetDesc/M86MCTargetDesc.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>

#if defined(M86_DEBUG_PRINTOUT)
#define M86_START_FUNCTION()                                                   \
  do {                                                                         \
    llvm::errs().changeColor(llvm::raw_ostream::RED);                          \
    llvm::errs() << "[START FUNCTION]" << ' ' << __FILE__ << '(' << __LINE__   \
                                                                               \
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
namespace M86 {
enum Mnemonic {
  RF = M86::R0,   // frame pointer
  RA = M86::R1,   // return address
  RI = M86::R2,   // immediate value
  RU1 = M86::R3,  // user-friendly register 1
  RU2 = M86::R4,  // user-friendly register 2
  RU3 = M86::R5,  // user-friendly register 3
  RU4 = M86::R6,  // user-friendly register 4
  RU5 = M86::R7,  // user-friendly register 5
  RU6 = M86::R8,  // user-friendly register 6
  RU7 = M86::R9,  // user-friendly register 7
  RU8 = M86::R10, // user-friendly register 8
  RS = M86::R11,  // stack pointer
  RG = M86::R12,  // global register
  RV = M86::R13,  // return value
  RB = M86::R14,  // base stack pointer
  RY = M86::R15   // currently not-used
};
} // namespace M86
} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86_H
