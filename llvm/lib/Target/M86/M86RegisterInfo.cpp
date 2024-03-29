#include "M86RegisterInfo.h"

#include "M86GenRegisterInfo.inc"

llvm::M86RegisterInfo::M86RegisterInfo()
    : llvm::M86GenRegisterInfo(llvm::M86::R0) {}
