//===-- SimELFObjectWriter.cpp - Sim ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

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

using namespace llvm;

namespace {
class M86ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  M86ELFObjectWriter(bool Is64Bit, uint8_t OSABI)
      : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_M86,
                                /*HasRelocationAddend*/ true) {
    M86_DEBUG_FUNCTION();
  }

  ~M86ELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;

  bool needsRelocateWithSymbol(const MCValue &Val, const MCSymbol &Sym,
                               unsigned Type) const override;
};
} // namespace

unsigned M86ELFObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                                          const MCFixup &Fixup,
                                          bool IsPCRel) const {
  M86_DEBUG_FUNCTION();
  MCFixupKind Kind = Fixup.getKind();
  if (Kind >= FirstLiteralRelocationKind)
    return Kind - FirstLiteralRelocationKind;

  llvm_unreachable("Unimplemented fixup -> relocation");
}

bool M86ELFObjectWriter::needsRelocateWithSymbol(const MCValue &,
                                                 const MCSymbol &,
                                                 unsigned Type) const {
  M86_DEBUG_FUNCTION();
  return false;
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createM86ELFObjectWriter(bool Is64Bit, uint8_t OSABI) {
  M86_DEBUG_FUNCTION();
  return std::make_unique<M86ELFObjectWriter>(Is64Bit, OSABI);
}
