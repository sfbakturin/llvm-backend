#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCCODEEMITTER_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCCODEEMITTER_H

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

namespace llvm {

class M86MCCodeEmitter : public MCCodeEmitter {
public:
  M86MCCodeEmitter(const MCInstrInfo &, MCContext &ctx);
  M86MCCodeEmitter(const M86MCCodeEmitter &) = delete;
  M86MCCodeEmitter &operator=(const M86MCCodeEmitter &) = delete;

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
  unsigned getImmOpValue(const MCInst &MI, unsigned OpNo,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const;

  /// getBranchTargetImmOpValue - Return binary encoding of the branch
  /// target operand. If the machine operand requires relocation,
  /// record the relocation and return zero.
  unsigned getBranchTargetOpValue(const MCInst &MI, unsigned OpNo,
                                  SmallVectorImpl<MCFixup> &Fixups,
                                  const MCSubtargetInfo &STI) const;

private:
  MCContext &Ctx;
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCCODEEMITTER_H
