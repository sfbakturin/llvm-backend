#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCTARGETDESC_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCTARGETDESC_H

// Defines symbolic names for M86 registers.  This defines a mapping from
// register name to register number.
//

#include <M86Info.h>

#define GET_REGINFO_ENUM
#include <M86GenRegisterInfo.inc>

// Defines symbolic names for the M86 instructions.
#define GET_INSTRINFO_ENUM
#include <M86GenInstrInfo.inc>

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCTARGETDESC_H
