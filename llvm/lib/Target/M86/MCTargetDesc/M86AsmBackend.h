#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ASMBACKEND_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ASMBACKEND_H

#include <M86.h>
#include <MCTargetDesc/M86MCTargetDesc.h>
#include <cstdint>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/bit.h>
#include <llvm/MC/MCAsmBackend.h>
#include <llvm/MC/MCELFObjectWriter.h>
#include <llvm/MC/MCExpr.h>
#include <llvm/MC/MCFixup.h>
#include <llvm/MC/MCFixupKindInfo.h>
#include <llvm/MC/MCFragment.h>
#include <llvm/MC/MCObjectWriter.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/MCTargetOptions.h>
#include <llvm/MC/MCValue.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/EndianStream.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TargetParser/Triple.h>
#include <memory>

namespace llvm {

class M86AsmBackend : public MCAsmBackend {
protected:
  const Target &TheTarget;

public:
  M86AsmBackend(const Target &T);

  unsigned getNumFixupKinds() const override;

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override;

  /// fixupNeedsRelaxation - Target specific predicate for whether a given
  /// fixup requires the associated instruction to be relaxed.
  bool fixupNeedsRelaxation(const MCFixup &Fixup, std::uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override;

  bool writeNopData(raw_ostream &OS, std::uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ASMBACKEND_H
