#include <M86RegisterInfo.h>

llvm::M86RegisterInfo::M86RegisterInfo()
    : llvm::M86GenRegisterInfo(llvm::M86::R0) {}
