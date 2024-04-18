#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86FIXUPKINDS_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86FIXUPKINDS_H

#include <llvm/MC/MCFixup.h>

namespace llvm {
namespace M86 {
// Although most of the current fixup types reflect a unique relocation
// one can have multiple fixup types for a given relocation and thus need
// to be uniquely named.
//
// This table *must* be in the same order of
// MCFixupKindInfo Infos[m86::NumTargetFixupKinds]
// in M86AsmBackend.cpp.
//
enum Fixups {
  FIXUP_M86_PC16 = FirstTargetFixupKind,
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace M86
} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86FIXUPKINDS_H
