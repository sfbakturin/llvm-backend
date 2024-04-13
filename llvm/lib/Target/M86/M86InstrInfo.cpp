#include <M86.h>
#include <M86InstrInfo.h>

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include <M86GenInstrInfo.inc>

#define DEBUG_TYPE "M86-inst-info"

llvm::M86InstrInfo::M86InstrInfo() : llvm::M86GenInstrInfo() {
  M86_DEBUG_FUNCTION();
}
