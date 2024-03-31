#ifndef LLVM_LIB_TARGET_M86_M86INSTRINFO_H
#define LLVM_LIB_TARGET_M86_M86INSTRINFO_H

#include <M86RegisterInfo.h>
#include <MCTargetDesc/M86Info.h>
#include <llvm/CodeGen/TargetInstrInfo.h>

#define GET_INSTRINFO_HEADER
#include <M86GenInstrInfo.inc>

namespace llvm {

class M86Subtarget;

class M86InstrInfo : public M86GenInstrInfo {
public:
  M86InstrInfo();
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86INSTRINFO_H
