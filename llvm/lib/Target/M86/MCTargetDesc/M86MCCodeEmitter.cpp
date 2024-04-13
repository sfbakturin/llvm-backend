//===-- SimMCCodeEmitter.cpp - Convert Sim code to machine code -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the SimMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include <M86.h>
#include <MCTargetDesc/M86MCTargetDesc.h>
#include <cassert>
#include <cstdint>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/ADT/bit.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCCodeEmitter.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCExpr.h>
#include <llvm/MC/MCFixup.h>
#include <llvm/MC/MCInst.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCObjectFileInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/MCSymbol.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/Endian.h>
#include <llvm/Support/EndianStream.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/TargetParser/SubtargetFeature.h>

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace {

class M86MCCodeEmitter : public MCCodeEmitter {
  MCContext &Ctx;

public:
  M86MCCodeEmitter(const MCInstrInfo &, MCContext &ctx) : Ctx(ctx) {}
  M86MCCodeEmitter(const M86MCCodeEmitter &) = delete;
  M86MCCodeEmitter &operator=(const M86MCCodeEmitter &) = delete;
  ~M86MCCodeEmitter() override = default;

  void encodeInstruction(const MCInst &MI, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  // getBinaryCodeForInstr - TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// getMachineOpValue - Return binary encoding of operand. If the machine
  /// operand requires relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;
  unsigned getM86ImmOpValue(const MCInst &MI, unsigned OpNo,
                            SmallVectorImpl<MCFixup> &Fixups,
                            const MCSubtargetInfo &STI) const;
};

} // end anonymous namespace

void M86MCCodeEmitter::encodeInstruction(const MCInst &MI,
                                         SmallVectorImpl<char> &CB,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  M86_DEBUG_FUNCTION();
  unsigned Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  support::endian::write(CB, Bits, llvm::endianness::little);

  ++MCNumEmitted; // Keep track of the # of mi's emitted.
}

unsigned M86MCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                             const MCOperand &MO,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  M86_DEBUG_FUNCTION();
  if (MO.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());

  if (MO.isImm())
    return MO.getImm();

  assert(MO.isExpr());
  const MCExpr *Expr = MO.getExpr();

  int64_t Res;
  if (Expr->evaluateAsAbsolute(Res))
    return Res;

  llvm_unreachable("Unhandled expression!");
  return 0;
}

unsigned M86MCCodeEmitter::getM86ImmOpValue(const MCInst &MI, unsigned OpNo,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  M86_DEBUG_FUNCTION();
  const MCOperand &MO = MI.getOperand(OpNo);
  if (MO.isImm())
    return MO.getImm();

  assert(MO.isExpr() &&
         "getM86ImmOpValue expects only expressions or an immediate");

  const MCExpr *Expr = MO.getExpr();

  // Constant value, no fixup is needed
  if (const MCConstantExpr *CE = dyn_cast<MCConstantExpr>(Expr))
    return CE->getValue();

  return 0;
}

#include "M86GenMCCodeEmitter.inc"

MCCodeEmitter *llvm::createM86MCCodeEmitter(const MCInstrInfo &MCII,
                                            MCContext &Ctx) {
  M86_DEBUG_FUNCTION();
  return new M86MCCodeEmitter(MCII, Ctx);
}
