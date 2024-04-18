#include <M86.h>
#include <MCTargetDesc/M86FixupKinds.h>
#include <MCTargetDesc/M86MCCodeEmitter.h>
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

llvm::M86MCCodeEmitter::M86MCCodeEmitter(const llvm::MCInstrInfo &,
                                         llvm::MCContext &ctx)
    : Ctx(ctx) {}

void llvm::M86MCCodeEmitter::encodeInstruction(
    const llvm::MCInst &MI, llvm::SmallVectorImpl<char> &CB,
    llvm::SmallVectorImpl<llvm::MCFixup> &Fixups,
    const llvm::MCSubtargetInfo &STI) const {
  M86_START_FUNCTION();

  unsigned Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  llvm::support::endian::write(CB, Bits, llvm::endianness::little);

  ++MCNumEmitted; // Keep track of the # of mi's emitted.

  M86_END_FUNCTION();
}

unsigned llvm::M86MCCodeEmitter::getMachineOpValue(
    const llvm::MCInst &MI, const llvm::MCOperand &MO,
    llvm::SmallVectorImpl<llvm::MCFixup> &Fixups,
    const llvm::MCSubtargetInfo &STI) const {
  M86_START_FUNCTION();

  if (MO.isReg()) {
    M86_END_FUNCTION();
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());
  }

  if (MO.isImm()) {
    M86_END_FUNCTION();
    return MO.getImm();
  }

  assert(MO.isExpr());
  const llvm::MCExpr *Expr = MO.getExpr();

  std::int64_t Res;
  if (Expr->evaluateAsAbsolute(Res)) {
    M86_END_FUNCTION();
    return Res;
  }

  llvm_unreachable("Unhandled expression!");

  M86_END_FUNCTION();

  return 0;
}

unsigned llvm::M86MCCodeEmitter::getImmOpValue(
    const llvm::MCInst &MI, unsigned OpNo,
    llvm::SmallVectorImpl<llvm::MCFixup> &Fixups,
    const llvm::MCSubtargetInfo &STI) const {
  M86_START_FUNCTION();

  const llvm::MCOperand &MO = MI.getOperand(OpNo);
  if (MO.isImm()) {
    M86_END_FUNCTION();
    return MO.getImm();
  }

  assert(MO.isExpr() &&
         "getM86ImmOpValue expects only expressions or an immediate");

  const llvm::MCExpr *Expr = MO.getExpr();

  // Constant value, no fixup is needed
  if (const llvm::MCConstantExpr *CE = dyn_cast<MCConstantExpr>(Expr)) {
    M86_END_FUNCTION();
    return CE->getValue();
  }

  M86_END_FUNCTION();

  return 0;
}

unsigned llvm::M86MCCodeEmitter::getBranchTargetOpValue(
    const MCInst &MI, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &STI) const {
  M86_START_FUNCTION();

  const llvm::MCOperand &MO = MI.getOperand(OpNo);

  // If the destination is an immediate, divide by 4.
  if (MO.isImm())
    return MO.getImm() / 4;

  assert(MO.isExpr() &&
         "getBranchTargetImmOpValue expects only expressions or immediates");

  Fixups.push_back(
      MCFixup::create(0, MO.getExpr(), MCFixupKind(llvm::M86::FIXUP_M86_PC16)));
  return 0;
}

#include <M86GenMCCodeEmitter.inc>

llvm::MCCodeEmitter *llvm::createM86MCCodeEmitter(const llvm::MCInstrInfo &MCII,
                                                  llvm::MCContext &Ctx) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return new llvm::M86MCCodeEmitter(MCII, Ctx);
}
