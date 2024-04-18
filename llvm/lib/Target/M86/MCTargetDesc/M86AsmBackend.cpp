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

llvm::M86AsmBackend::M86AsmBackend(const llvm::Target &T)
    : llvm::MCAsmBackend(llvm::endianness::little), TheTarget(T) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

unsigned llvm::M86AsmBackend::getNumFixupKinds() const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return llvm::M86::Fixups::NumTargetFixupKinds;
}

const llvm::MCFixupKindInfo &
llvm::M86AsmBackend::getFixupKindInfo(llvm::MCFixupKind Kind) const {
  M86_START_FUNCTION();

  const static llvm::MCFixupKindInfo
      InfosLE[llvm::M86::Fixups::NumTargetFixupKinds] = {
          // name      offset bits  flags
          {"FIXUP_M86_PC16", 0, 16, llvm::MCFixupKindInfo::FKF_IsPCRel},
      };

  // Fixup kinds from .reloc directive are like R_SPARC_NONE. They do
  // not require any extra processing.
  if (Kind >= FirstLiteralRelocationKind) {
    M86_END_FUNCTION();
    return llvm::MCAsmBackend::getFixupKindInfo(FK_NONE);
  }

  if (Kind < FirstTargetFixupKind) {
    M86_END_FUNCTION();
    return llvm::MCAsmBackend::getFixupKindInfo(Kind);
  }

  assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
         "Invalid kind!");
  M86_END_FUNCTION();
  return InfosLE[Kind - FirstTargetFixupKind];
}

bool llvm::M86AsmBackend::fixupNeedsRelaxation(
    const llvm::MCFixup &Fixup, std::uint64_t Value,
    const llvm::MCRelaxableFragment *DF,
    const llvm::MCAsmLayout &Layout) const {
  M86_START_FUNCTION();
  llvm_unreachable("fixupNeedsRelaxation() unimplemented");
  M86_END_FUNCTION();
  return false;
}

bool llvm::M86AsmBackend::writeNopData(llvm::raw_ostream &OS,
                                       std::uint64_t Count,
                                       const llvm::MCSubtargetInfo *STI) const {
  M86_START_FUNCTION();
  OS.write_zeros(Count);
  M86_END_FUNCTION();
  return true;
}

llvm::MCAsmBackend *llvm::createM86AsmBackend(
    const llvm::Target &T, const llvm::MCSubtargetInfo &STI,
    const llvm::MCRegisterInfo &MRI, const llvm::MCTargetOptions &Options) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return new llvm::M86ELFAsmBackend(T, STI.getTargetTriple().getOS());
}
