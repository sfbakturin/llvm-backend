#include <M86.h>
#include <M86InstrLowering.h>
#include <llvm/CodeGen/AsmPrinter.h>
#include <llvm/CodeGen/MachineInstr.h>
#include <llvm/CodeGen/MachineOperand.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCExpr.h>
#include <llvm/MC/MCInst.h>
#include <llvm/MC/MCSymbol.h>
#include <llvm/Support/ErrorHandling.h>

using namespace llvm;

static llvm::MCOperand lowerSymbolOperand(const llvm::MachineOperand &MO,
                                          llvm::MCSymbol *Sym,
                                          const llvm::AsmPrinter &AP) {
  M86_START_FUNCTION();

  llvm::MCContext &Ctx = AP.OutContext;

  const llvm::MCExpr *ME =
      llvm::MCSymbolRefExpr::create(Sym, llvm::MCSymbolRefExpr::VK_None, Ctx);

  if (!MO.isJTI() && !MO.isMBB() && MO.getOffset()) {
    ME = llvm::MCBinaryExpr::createAdd(
        ME, llvm::MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);
  }

  M86_END_FUNCTION();

  return llvm::MCOperand::createExpr(ME);
}

bool llvm::LowerM86MachineOperandToMCOperand(const llvm::MachineOperand &MO,
                                             llvm::MCOperand &MCOp,
                                             const llvm::AsmPrinter &AP) {
  M86_START_FUNCTION();

  switch (MO.getType()) {
  default:
    llvm::report_fatal_error(
        "LowerM86MachineInstrToMCInst: unknown operand type");
  case llvm::MachineOperand::MO_Register: {
    // Ignore all implicit register operands.
    if (MO.isImplicit()) {
      M86_END_FUNCTION();
      return false;
    }
    MCOp = llvm::MCOperand::createReg(MO.getReg());
    break;
  }
  case llvm::MachineOperand::MO_RegisterMask: {
    // Regmasks are like implicit defs.
    M86_END_FUNCTION();
    return false;
  }
  case llvm::MachineOperand::MO_Immediate: {
    MCOp = llvm::MCOperand::createImm(MO.getImm());
    break;
  }
  case llvm::MachineOperand::MO_MachineBasicBlock: {
    MCOp = lowerSymbolOperand(MO, MO.getMBB()->getSymbol(), AP);
    break;
  }
  case llvm::MachineOperand::MO_GlobalAddress: {
    MCOp = lowerSymbolOperand(MO, AP.getSymbolPreferLocal(*MO.getGlobal()), AP);
    break;
  }
  case llvm::MachineOperand::MO_BlockAddress: {
    MCOp = lowerSymbolOperand(
        MO, AP.GetBlockAddressSymbol(MO.getBlockAddress()), AP);
    break;
  }
  case llvm::MachineOperand::MO_ExternalSymbol: {
    MCOp = lowerSymbolOperand(
        MO, AP.GetExternalSymbolSymbol(MO.getSymbolName()), AP);
    break;
  }
  case llvm::MachineOperand::MO_ConstantPoolIndex: {
    MCOp = lowerSymbolOperand(MO, AP.GetCPISymbol(MO.getIndex()), AP);
    break;
  }
  case llvm::MachineOperand::MO_JumpTableIndex: {
    MCOp = lowerSymbolOperand(MO, AP.GetJTISymbol(MO.getIndex()), AP);
    break;
  }
  }

  M86_END_FUNCTION();

  return true;
}

bool llvm::lowerM86MachineInstrToMCInst(const llvm::MachineInstr *MI,
                                        llvm::MCInst &OutMI,
                                        llvm::AsmPrinter &AP) {
  M86_START_FUNCTION();

  OutMI.setOpcode(MI->getOpcode());

  for (const llvm::MachineOperand &MO : MI->operands()) {
    llvm::MCOperand MCOp;
    if (llvm::LowerM86MachineOperandToMCOperand(MO, MCOp, AP))
      OutMI.addOperand(MCOp);
  }

  M86_END_FUNCTION();

  return false;
}
