//===-- SimAsmBackend.cpp - Sim Assembler Backend ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

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

using namespace llvm;

namespace {
class M86AsmBackend : public MCAsmBackend {
protected:
  const Target &TheTarget;

public:
  M86AsmBackend(const Target &T)
      : MCAsmBackend(llvm::endianness::little), TheTarget(T) {
    M86_DEBUG_FUNCTION();
  }

  unsigned getNumFixupKinds() const override {
    M86_DEBUG_FUNCTION();
    return 0;
  }

  /// fixupNeedsRelaxation - Target specific predicate for whether a given
  /// fixup requires the associated instruction to be relaxed.
  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override {
    M86_DEBUG_FUNCTION();
    llvm_unreachable("fixupNeedsRelaxation() unimplemented");
    return false;
  }

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override {
    M86_DEBUG_FUNCTION();
    // Cannot emit NOP with size not multiple of 32 bits.
    if (Count % 4 != 0)
      return false;

    uint64_t NumNops = Count / 4;
    for (uint64_t i = 0; i != NumNops; ++i)
      support::endian::write<uint32_t>(OS, 0x01000000, Endian);

    return true;
  }
};

class ELFSimAsmBackend : public M86AsmBackend {
  Triple::OSType OSType;

public:
  ELFSimAsmBackend(const Target &T, Triple::OSType OSType)
      : M86AsmBackend(T), OSType(OSType) {
    M86_DEBUG_FUNCTION();
  }

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override {
    M86_DEBUG_FUNCTION();
    return;
  }

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override {
    M86_DEBUG_FUNCTION();
    uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(OSType);
    return createM86ELFObjectWriter(false, OSABI);
  }
};

} // end anonymous namespace

MCAsmBackend *llvm::createM86AsmBackend(const Target &T,
                                        const MCSubtargetInfo &STI,
                                        const MCRegisterInfo &MRI,
                                        const MCTargetOptions &Options) {
  M86_DEBUG_FUNCTION();
  return new ELFSimAsmBackend(T, STI.getTargetTriple().getOS());
}
