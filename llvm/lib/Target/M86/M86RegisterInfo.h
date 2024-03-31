#ifndef LLVM_LIB_TARGET_M86_M86REGISTERINFO_H
#define LLVM_LIB_TARGET_M86_M86REGISTERINFO_H

#define GET_REGINFO_HEADER
#define GET_REGINFO_ENUM
#include <M86GenRegisterInfo.inc>

namespace llvm {

struct M86RegisterInfo : public M86GenRegisterInfo {
public:
  M86RegisterInfo();
};

} // namespace llvm

#endif
