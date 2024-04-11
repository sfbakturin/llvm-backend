#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ELFMCASMINFO_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ELFMCASMINFO_H

#include <llvm/MC/MCAsmInfoELF.h>

namespace llvm {

class Triple;

class M86ELFMCAsmInfo : public MCAsmInfoELF {
public:
  explicit M86ELFMCAsmInfo(const Triple &TheTriple);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ELFMCASMINFO_H
