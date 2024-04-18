#include <M86.h>
#include <MCTargetDesc/M86AsmBackend.h>
#include <MCTargetDesc/M86ELFAsmBackend.h>
#include <MCTargetDesc/M86FixupKinds.h>
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

using namespace llvm;

llvm::M86ELFAsmBackend::M86ELFAsmBackend(const llvm::Target &T,
                                         llvm::Triple::OSType OSType)
    : llvm::M86AsmBackend(T), OSType(OSType) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

void llvm::M86ELFAsmBackend::applyFixup(
    const llvm::MCAssembler &Asm, const llvm::MCFixup &Fixup,
    const llvm::MCValue &Target, llvm::MutableArrayRef<char> Data,
    std::uint64_t Value, bool IsResolved,
    const llvm::MCSubtargetInfo *STI) const {
  M86_START_FUNCTION();

  unsigned NumBytes = 0;

  switch (Fixup.getKind()) {
  default:
    M86_END_FUNCTION();
    return;
  case llvm::M86::FIXUP_M86_PC16:
    // Forcing a signed division because Value can be negative.
    Value /= 4;
    NumBytes = 2;
    break;
  }

  unsigned Offset = Fixup.getOffset();
  // For each byte of the fragment that the fixup touches, mask in the bits
  // from the fixup value. The Value has been "split up" into the
  // appropriate bitfields above.
  for (unsigned I = 0; I != NumBytes; ++I) {
    Data[Offset + I] |= std::uint8_t((Value >> (I * 8)) & 0xff);
  }

  M86_END_FUNCTION();
}

std::unique_ptr<llvm::MCObjectTargetWriter>
llvm::M86ELFAsmBackend::createObjectTargetWriter() const {
  M86_START_FUNCTION();
  std::uint8_t OSABI = llvm::MCELFObjectTargetWriter::getOSABI(OSType);
  M86_END_FUNCTION();
  return llvm::createM86ELFObjectWriter(false, OSABI);
}
