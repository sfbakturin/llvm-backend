#include <M86.h>
#include <MCTargetDesc/M86ELFObjectWriter.h>
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

using namespace llvm;

llvm::M86ELFObjectWriter::M86ELFObjectWriter(bool Is64Bit, std::uint8_t OSABI)
    : llvm::MCELFObjectTargetWriter(Is64Bit, OSABI, llvm::ELF::EM_M86,
                                    /*HasRelocationAddend*/ true) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

unsigned llvm::M86ELFObjectWriter::getRelocType(llvm::MCContext &Ctx,
                                                const llvm::MCValue &Target,
                                                const llvm::MCFixup &Fixup,
                                                bool IsPCRel) const {
  M86_START_FUNCTION();

  llvm::MCFixupKind Kind = Fixup.getKind();

  if (Kind >= FirstLiteralRelocationKind) {
    M86_END_FUNCTION();
    return Kind - FirstLiteralRelocationKind;
  }

  if (Kind == FirstTargetFixupKind) {
    // TODO rewrite taht it uses  directly value from table in  InfosLE
    M86_END_FUNCTION();
    return 0;
  }

  M86_END_FUNCTION();

  llvm_unreachable("Unimplemented fixup -> relocation");
}

bool llvm::M86ELFObjectWriter::needsRelocateWithSymbol(const llvm::MCValue &,
                                                       const llvm::MCSymbol &,
                                                       unsigned Type) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return false;
}

std::unique_ptr<llvm::MCObjectTargetWriter>
llvm::createM86ELFObjectWriter(bool Is64Bit, std::uint8_t OSABI) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return std::make_unique<llvm::M86ELFObjectWriter>(Is64Bit, OSABI);
}
