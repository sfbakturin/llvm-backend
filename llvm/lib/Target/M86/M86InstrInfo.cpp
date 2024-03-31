#include <M86InstrInfo.h>
#include <llvm/CodeGen/MachineFrameInfo.h>
#include <llvm/CodeGen/MachineInstrBuilder.h>
#include <llvm/CodeGen/MachineMemOperand.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/ErrorHandling.h>

#define GET_INSTRINFO_CTOR_DTOR
#include <M86GenInstrInfo.inc>

#define DEBUG_TYPE "M86-inst-info"

llvm::M86InstrInfo::M86InstrInfo() : llvm::M86GenInstrInfo() {}
