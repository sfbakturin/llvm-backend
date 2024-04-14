#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ELFOBJECTWRITER_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ELFOBJECTWRITER_H

#include <M86.h>
#include <MCTargetDesc/M86MCTargetDesc.h>
#include <cstdint>
#include <llvm/ADT/STLExtras.h>
#include <llvm/BinaryFormat/ELF.h>
#include <llvm/MC/MCELFObjectWriter.h>
#include <llvm/MC/MCExpr.h>
#include <llvm/MC/MCFixup.h>
#include <llvm/MC/MCObjectWriter.h>
#include <llvm/MC/MCSymbol.h>
#include <llvm/MC/MCValue.h>
#include <llvm/Support/ErrorHandling.h>
#include <memory>

namespace llvm {

class M86ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  M86ELFObjectWriter(bool Is64Bit, std::uint8_t OSABI);

  ~M86ELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;

  bool needsRelocateWithSymbol(const MCValue &Val, const MCSymbol &Sym,
                               unsigned Type) const override;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86ELFOBJECTWRITER_H
