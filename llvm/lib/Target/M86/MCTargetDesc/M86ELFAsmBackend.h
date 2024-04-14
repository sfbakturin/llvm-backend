#ifndef M86ELFASMBACKEND_H
#define M86ELFASMBACKEND_H

#include <M86.h>
#include <MCTargetDesc/M86AsmBackend.h>
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

class M86ELFAsmBackend : public M86AsmBackend {
public:
  M86ELFAsmBackend(const Target &T, Triple::OSType OSType);

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  std::uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override;

private:
  Triple::OSType OSType;
};

} // namespace llvm

#endif // M86ELFASMBACKEND_H
