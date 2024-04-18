#include <M86.h>
#include <M86ISelDagToDag.h>
#include <M86InstrLowering.h>
#include <M86Subtarget.h>
#include <M86TargetMachine.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/CodeGen/MachineFunction.h>
#include <llvm/CodeGen/SelectionDAGISel.h>
#include <llvm/CodeGen/SelectionDAGNodes.h>
#include <llvm/Pass.h>
#include <llvm/Support/Debug.h>

using namespace llvm;

#define DEBUG_TYPE "M86-isel"

char llvm::M86DAGToDAGISel::ID = 0;

llvm::M86DAGToDAGISel::M86DAGToDAGISel(llvm::M86TargetMachine &tm)
    : llvm::SelectionDAGISel(ID, tm) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

bool llvm::M86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction &MF) {
  M86_START_FUNCTION();
  Subtarget = &MF.getSubtarget<llvm::M86Subtarget>();
  M86_END_FUNCTION();
  return llvm::SelectionDAGISel::runOnMachineFunction(MF);
}

llvm::StringRef llvm::M86DAGToDAGISel::getPassName() const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return "M86 DAG->DAG Pattern Instruction Selection";
}

/// This pass converts a legalized DAG into a Sim-specific DAG, ready for
/// instruction scheduling.
llvm::FunctionPass *llvm::createM86ISelDag(llvm::M86TargetMachine &TM) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return new llvm::M86DAGToDAGISel(TM);
}

void llvm::M86DAGToDAGISel::Select(llvm::SDNode *Node) {
  M86_START_FUNCTION();
  if (Node->isMachineOpcode()) {
    LLVM_DEBUG(llvm::dbgs() << "== "; Node->dump(CurDAG); llvm::dbgs() << "\n");
    Node->setNodeId(-1);
    M86_END_FUNCTION();
    return;
  }
  unsigned Opcode = Node->getOpcode();
  llvm::SDLoc DL(Node);
  llvm::MVT VT = Node->getSimpleValueType(0);

  switch (Opcode) {
  case llvm::ISD::FrameIndex: {
    llvm::SDValue Imm = CurDAG->getTargetConstant(0, DL, llvm::MVT::i32);
    int FI = llvm::cast<llvm::FrameIndexSDNode>(Node)->getIndex();
    llvm::SDValue TFI = CurDAG->getTargetFrameIndex(FI, VT);
    ReplaceNode(Node,
                CurDAG->getMachineNode(llvm::M86::ADDI, DL, VT, TFI, Imm));
    M86_END_FUNCTION();
    return;
  }
  }
  SelectCode(Node);
  M86_END_FUNCTION();
}

bool llvm::M86DAGToDAGISel::SelectFIAddress(llvm::SDValue Addr,
                                            llvm::SDValue &Base) {
  M86_START_FUNCTION();
  if (auto *FIN = llvm::dyn_cast<llvm::FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), llvm::MVT::i32);
    M86_END_FUNCTION();
    return true;
  }
  M86_END_FUNCTION();
  return false;
}

bool llvm::M86DAGToDAGISel::SelectBaseAddress(llvm::SDValue Addr,
                                              llvm::SDValue &Base) {
  M86_START_FUNCTION();
  if (auto *FIN = llvm::dyn_cast<llvm::FrameIndexSDNode>(Addr))
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), llvm::MVT::i32);
  else
    Base = Addr;
  M86_END_FUNCTION();
  return true;
}
