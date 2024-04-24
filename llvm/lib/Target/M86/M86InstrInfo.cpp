#include <M86.h>
#include <M86InstrInfo.h>
#include <llvm/CodeGen/MachineFrameInfo.h>
#include <llvm/CodeGen/MachineInstrBuilder.h>
#include <llvm/CodeGen/MachineMemOperand.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/ErrorHandling.h>

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include <M86GenInstrInfo.inc>

#define DEBUG_TYPE "M86-inst-info"

llvm::M86InstrInfo::M86InstrInfo()
    : M86GenInstrInfo(M86::ADJCALLSTACKDOWN, M86::ADJCALLSTACKUP) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

unsigned llvm::M86InstrInfo::isLoadFromStackSlot(const llvm::MachineInstr &MI,
                                                 int &FrameIndex) const {
  M86_START_FUNCTION();
  if (MI.getOpcode() == llvm::M86::MEMLD && MI.getOperand(1).isFI() &&
      MI.getOperand(2).isImm() && MI.getOperand(2).getImm() == 0) {
    FrameIndex = MI.getOperand(1).getIndex();
    M86_END_FUNCTION();
    return MI.getOperand(0).getReg();
  }
  M86_END_FUNCTION();
  return 0;
}

unsigned llvm::M86InstrInfo::isStoreToStackSlot(const llvm::MachineInstr &MI,
                                                int &FrameIndex) const {
  M86_START_FUNCTION();
  if (MI.getOpcode() == llvm::M86::MEMST &&
      (MI.getOperand(1).isFI()) && // is a stack slot
      (MI.getOperand(2).isImm()) && MI.getOperand(2).getImm() == 0) {
    FrameIndex = MI.getOperand(1).getIndex();
    M86_END_FUNCTION();
    return MI.getOperand(0).getReg();
  }
  M86_END_FUNCTION();
  return 0;
}

static llvm::M86CC::CondCode getCondFromBranchOpc(unsigned Opc) {
  M86_START_FUNCTION();
  switch (Opc) {
  default:
    M86_END_FUNCTION();
    return llvm::M86CC::INVALID;
  case llvm::M86::JEQ:
    M86_END_FUNCTION();
    return llvm::M86CC::EQ;
  case llvm::M86::JGT:
    M86_END_FUNCTION();
    return llvm::M86CC::GT;
  case llvm::M86::JGE:
    M86_END_FUNCTION();
    return llvm::M86CC::GE;
  case llvm::M86::JLT:
    M86_END_FUNCTION();
    return llvm::M86CC::LT;
  case llvm::M86::JLE:
    M86_END_FUNCTION();
    return llvm::M86CC::LE;
  case llvm::M86::JNE:
    M86_END_FUNCTION();
    return llvm::M86CC::NE;
  case llvm::M86::JUEQ:
    M86_END_FUNCTION();
    return llvm::M86CC::UEQ;
  case llvm::M86::JUGT:
    M86_END_FUNCTION();
    return llvm::M86CC::UGT;
  case llvm::M86::JUGE:
    M86_END_FUNCTION();
    return llvm::M86CC::UGE;
  case llvm::M86::JULT:
    M86_END_FUNCTION();
    return llvm::M86CC::ULT;
  case llvm::M86::JULE:
    M86_END_FUNCTION();
    return llvm::M86CC::ULE;
  case llvm::M86::JUNE:
    M86_END_FUNCTION();
    return llvm::M86CC::UNE;
  }
}

static void parseCondBranch(llvm::MachineInstr &LastInst,
                            llvm::MachineBasicBlock *&Target,
                            llvm::SmallVectorImpl<llvm::MachineOperand> &Cond) {
  M86_START_FUNCTION();
  // Block ends with fall-through condbranch.
  assert(LastInst.getDesc().isConditionalBranch() &&
         "Unknown conditional branch");
  Target = LastInst.getOperand(2).getMBB();
  unsigned CC = getCondFromBranchOpc(LastInst.getOpcode());
  Cond.push_back(llvm::MachineOperand::CreateImm(CC));
  Cond.push_back(LastInst.getOperand(0));
  Cond.push_back(LastInst.getOperand(1));
  M86_END_FUNCTION();
}

const llvm::MCInstrDesc &
llvm::M86InstrInfo::getBrCond(llvm::M86CC::CondCode CC) const {
  M86_START_FUNCTION();
  switch (CC) {
  default:
    M86_END_FUNCTION();
    llvm_unreachable("Unknown condition code!");
  case llvm::M86CC::EQ:
    M86_END_FUNCTION();
    return get(llvm::M86::JEQ);
  case llvm::M86CC::GT:
    M86_END_FUNCTION();
    return get(llvm::M86::JGT);
  case llvm::M86CC::GE:
    M86_END_FUNCTION();
    return get(llvm::M86::JGE);
  case llvm::M86CC::LT:
    M86_END_FUNCTION();
    return get(llvm::M86::JLT);
  case llvm::M86CC::LE:
    M86_END_FUNCTION();
    return get(llvm::M86::JLE);
  case llvm::M86CC::NE:
    M86_END_FUNCTION();
    return get(llvm::M86::JNE);
  case llvm::M86CC::UEQ:
    M86_END_FUNCTION();
    return get(llvm::M86::JUEQ);
  case llvm::M86CC::UGT:
    M86_END_FUNCTION();
    return get(llvm::M86::JUGT);
  case llvm::M86CC::UGE:
    M86_END_FUNCTION();
    return get(llvm::M86::JUGE);
  case llvm::M86CC::ULT:
    M86_END_FUNCTION();
    return get(llvm::M86::JULT);
  case llvm::M86CC::ULE:
    M86_END_FUNCTION();
    return get(llvm::M86::JULE);
  case llvm::M86CC::UNE:
    M86_END_FUNCTION();
    return get(llvm::M86::JUNE);
  }
}

llvm::M86CC::CondCode
llvm::M86CC::getOppositeBranchCondition(llvm::M86CC::CondCode CC) {
  M86_START_FUNCTION();
  switch (CC) {
  default:
    M86_END_FUNCTION();
    llvm_unreachable("Unrecognized conditional branch");
  case llvm::M86CC::EQ:
    M86_END_FUNCTION();
    return llvm::M86CC::NE;
  case llvm::M86CC::GT:
    M86_END_FUNCTION();
    return llvm::M86CC::LE;
  case llvm::M86CC::GE:
    M86_END_FUNCTION();
    return llvm::M86CC::LT;
  case llvm::M86CC::LT:
    M86_END_FUNCTION();
    return llvm::M86CC::GE;
  case llvm::M86CC::LE:
    M86_END_FUNCTION();
    return llvm::M86CC::GT;
  case llvm::M86CC::NE:
    M86_END_FUNCTION();
    return llvm::M86CC::EQ;
  case llvm::M86CC::UEQ:
    M86_END_FUNCTION();
    return llvm::M86CC::UNE;
  case llvm::M86CC::UGT:
    M86_END_FUNCTION();
    return llvm::M86CC::ULE;
  case llvm::M86CC::UGE:
    M86_END_FUNCTION();
    return llvm::M86CC::ULT;
  case llvm::M86CC::ULT:
    M86_END_FUNCTION();
    return llvm::M86CC::UGE;
  case llvm::M86CC::ULE:
    M86_END_FUNCTION();
    return llvm::M86CC::UGT;
  case llvm::M86CC::UNE:
    M86_END_FUNCTION();
    return llvm::M86CC::UEQ;
  }
}

bool llvm::M86InstrInfo::analyzeBranch(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock *&TBB,
    llvm::MachineBasicBlock *&FBB,
    llvm::SmallVectorImpl<llvm::MachineOperand> &Cond, bool AllowModify) const {
  M86_START_FUNCTION();

  TBB = FBB = nullptr;
  Cond.clear();

  // If the block has no terminators, it just falls into the block after it.
  llvm::MachineBasicBlock::iterator I = MBB.getLastNonDebugInstr();
  if (I == MBB.end() || !isUnpredicatedTerminator(*I)) {
    M86_END_FUNCTION();
    return false;
  }

  // Count the number of terminators and find the first unconditional or
  // indirect branch.
  llvm::MachineBasicBlock::iterator FirstUncondOrIndirectBr = MBB.end();
  int NumTerminators = 0;
  for (auto J = I.getReverse(); J != MBB.rend() && isUnpredicatedTerminator(*J);
       J++) {
    NumTerminators++;
    if (J->getDesc().isUnconditionalBranch() ||
        J->getDesc().isIndirectBranch()) {
      FirstUncondOrIndirectBr = J.getReverse();
    }
  }

  // If AllowModify is true, we can erase any terminators after
  // FirstUncondOrIndirectBR.
  if (AllowModify && FirstUncondOrIndirectBr != MBB.end()) {
    while (std::next(FirstUncondOrIndirectBr) != MBB.end()) {
      std::next(FirstUncondOrIndirectBr)->eraseFromParent();
      NumTerminators--;
    }
    I = FirstUncondOrIndirectBr;
  }

  // We can't handle blocks that end in an indirect branch.
  if (I->getDesc().isIndirectBranch()) {
    M86_END_FUNCTION();
    return true;
  }

  // We can't handle blocks with more than 2 terminators.
  if (NumTerminators > 2) {
    M86_END_FUNCTION();
    return true;
  }

  // Handle a single unconditional branch.
  if (NumTerminators == 1 && I->getDesc().isUnconditionalBranch()) {
    TBB = getBranchDestBlock(*I);
    M86_END_FUNCTION();
    return false;
  }

  // Handle a single conditional branch.
  if (NumTerminators == 1 && I->getDesc().isConditionalBranch()) {
    parseCondBranch(*I, TBB, Cond);
    M86_END_FUNCTION();
    return false;
  }

  // Handle a conditional branch followed by an unconditional branch.
  if (NumTerminators == 2 && std::prev(I)->getDesc().isConditionalBranch() &&
      I->getDesc().isUnconditionalBranch()) {
    parseCondBranch(*std::prev(I), TBB, Cond);
    FBB = getBranchDestBlock(*I);
    M86_END_FUNCTION();
    return false;
  }

  // Otherwise, we can't handle this.
  M86_END_FUNCTION();
  return true;
}

unsigned llvm::M86InstrInfo::removeBranch(llvm::MachineBasicBlock &MBB,
                                          int *BytesRemoved) const {
  M86_START_FUNCTION();

  if (BytesRemoved)
    *BytesRemoved = 0;

  llvm::MachineBasicBlock::iterator I = MBB.getLastNonDebugInstr();

  if (I == MBB.end()) {
    M86_END_FUNCTION();
    return 0;
  }

  if (!I->getDesc().isUnconditionalBranch() &&
      !I->getDesc().isConditionalBranch()) {
    M86_END_FUNCTION();
    return 0;
  }

  // Remove the branch.
  if (BytesRemoved)
    *BytesRemoved += getInstSizeInBytes(*I);
  I->eraseFromParent();

  I = MBB.end();

  if (I == MBB.begin()) {
    M86_END_FUNCTION();
    return 1;
  }
  --I;
  if (!I->getDesc().isConditionalBranch()) {
    M86_END_FUNCTION();
    return 1;
  }

  // Remove the branch.
  if (BytesRemoved)
    *BytesRemoved += getInstSizeInBytes(*I);
  I->eraseFromParent();
  M86_END_FUNCTION();
  return 2;
}

llvm::MachineBasicBlock *
llvm::M86InstrInfo::getBranchDestBlock(const llvm::MachineInstr &MI) const {
  M86_START_FUNCTION();
  assert(MI.getDesc().isBranch() && "Unexpected opcode!");
  M86_END_FUNCTION();
  return MI.getOperand(MI.getNumExplicitOperands() - 1).getMBB();
}

void llvm::M86InstrInfo::copyPhysReg(llvm::MachineBasicBlock &MBB,
                                     llvm::MachineBasicBlock::iterator MBBI,
                                     const llvm::DebugLoc &DL,
                                     llvm::MCRegister DstReg,
                                     llvm::MCRegister SrcReg,
                                     bool KillSrc) const {
  M86_START_FUNCTION();
  if (llvm::M86::GPRRegClass.contains(DstReg, SrcReg)) {
    BuildMI(MBB, MBBI, DL, get(llvm::M86::ADDI), DstReg)
        .addReg(SrcReg, getKillRegState(KillSrc))
        .addImm(0);
    M86_END_FUNCTION();
    return;
  }
  llvm_unreachable("can't copyPhysReg");
}

void llvm::M86InstrInfo::storeRegToStackSlot(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock::iterator I,
    llvm::Register SrcReg, bool IsKill, int FI,
    const llvm::TargetRegisterClass *RC, const llvm::TargetRegisterInfo *TRI,
    llvm::Register VReg) const {
  M86_START_FUNCTION();

  llvm::DebugLoc DL;
  if (I != MBB.end())
    DL = I->getDebugLoc();

  llvm::MachineFunction *MF = MBB.getParent();
  llvm::MachineFrameInfo &MFI = MF->getFrameInfo();

  llvm::MachineMemOperand *MMO =
      MF->getMachineMemOperand(llvm::MachinePointerInfo::getFixedStack(*MF, FI),
                               llvm::MachineMemOperand::MOStore,
                               MFI.getObjectSize(FI), MFI.getObjectAlign(FI));

  BuildMI(MBB, I, DL, get(llvm::M86::MEMST))
      .addReg(SrcReg, getKillRegState(IsKill))
      .addFrameIndex(FI)
      .addImm(0)
      .addMemOperand(MMO);

  M86_END_FUNCTION();
}

void llvm::M86InstrInfo::loadRegFromStackSlot(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock::iterator I,
    llvm::Register DstReg, int FI, const llvm::TargetRegisterClass *RC,
    const llvm::TargetRegisterInfo *TRI, llvm::Register VReg) const {
  M86_START_FUNCTION();

  llvm::DebugLoc DL;
  if (I != MBB.end())
    DL = I->getDebugLoc();

  llvm::MachineFunction *MF = MBB.getParent();
  llvm::MachineFrameInfo &MFI = MF->getFrameInfo();

  llvm::MachineMemOperand *MMO =
      MF->getMachineMemOperand(llvm::MachinePointerInfo::getFixedStack(*MF, FI),
                               llvm::MachineMemOperand::MOLoad,
                               MFI.getObjectSize(FI), MFI.getObjectAlign(FI));

  BuildMI(MBB, I, DL, get(llvm::M86::MEMLD), DstReg)
      .addFrameIndex(FI)
      .addImm(0)
      .addMemOperand(MMO);

  M86_END_FUNCTION();
}

bool llvm::M86InstrInfo::reverseBranchCondition(
    llvm::SmallVectorImpl<llvm::MachineOperand> &Cond) const {
  M86_START_FUNCTION();
  assert((Cond.size() == 3) && "Invalid branch condition!");
  auto CC = static_cast<llvm::M86CC::CondCode>(Cond[0].getImm());
  Cond[0].setImm(getOppositeBranchCondition(CC));
  M86_END_FUNCTION();
  return false;
}

unsigned llvm::M86InstrInfo::insertBranch(
    llvm::MachineBasicBlock &MBB, llvm::MachineBasicBlock *TBB,
    llvm::MachineBasicBlock *FBB, llvm::ArrayRef<llvm::MachineOperand> Cond,
    const llvm::DebugLoc &DL, int *BytesAdded) const {
  M86_START_FUNCTION();
  if (BytesAdded)
    *BytesAdded = 0;

  // Shouldn't be a fall through.
  assert(TBB && "insertBranch must not be told to insert a fallthrough");
  assert((Cond.size() == 3 || Cond.size() == 0) &&
         "Wrong number of components");

  // Unconditional branch.
  if (Cond.empty()) {
    llvm::MachineInstr &MI =
        *BuildMI(&MBB, DL, get(llvm::M86::JMPL)).addMBB(TBB);
    if (BytesAdded)
      *BytesAdded += getInstSizeInBytes(MI);
    M86_END_FUNCTION();
    return 1;
  }

  // Either a one or two-way conditional branch.
  auto CC = static_cast<llvm::M86CC::CondCode>(Cond[0].getImm());
  llvm::MachineInstr &CondMI =
      *BuildMI(&MBB, DL, getBrCond(CC)).add(Cond[1]).add(Cond[2]).addMBB(TBB);
  if (BytesAdded)
    *BytesAdded += getInstSizeInBytes(CondMI);

  // One-way conditional branch.
  if (!FBB) {
    M86_END_FUNCTION();
    return 1;
  }

  // Two-way conditional branch.
  llvm::MachineInstr &MI = *BuildMI(&MBB, DL, get(llvm::M86::JMPL)).addMBB(FBB);
  if (BytesAdded)
    *BytesAdded += getInstSizeInBytes(MI);
  M86_END_FUNCTION();
  return 2;
}

unsigned M86InstrInfo::getInstSizeInBytes(const llvm::MachineInstr &MI) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  llvm_unreachable("");
}

bool M86InstrInfo::getBaseAndOffsetPosition(const llvm::MachineInstr &MI,
                                            unsigned &BasePos,
                                            unsigned &OffsetPos) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  llvm_unreachable("");
}