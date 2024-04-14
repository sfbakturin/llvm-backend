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
}

bool llvm::M86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction &MF) {
  M86_START_FUNCTION();

  Subtarget = &MF.getSubtarget<llvm::M86Subtarget>();
  return llvm::SelectionDAGISel::runOnMachineFunction(MF);
}

llvm::StringRef llvm::M86DAGToDAGISel::getPassName() const {
  M86_START_FUNCTION();

  return "M86 DAG->DAG Pattern Instruction Selection";
}

/// This pass converts a legalized DAG into a Sim-specific DAG, ready for
/// instruction scheduling.
llvm::FunctionPass *llvm::createM86ISelDag(llvm::M86TargetMachine &TM) {
  M86_START_FUNCTION();

  return new llvm::M86DAGToDAGISel(TM);
}

void llvm::M86DAGToDAGISel::Select(llvm::SDNode *Node) {
  M86_START_FUNCTION();

  if (Node->isMachineOpcode()) {
    LLVM_DEBUG(llvm::dbgs() << "== "; Node->dump(CurDAG); llvm::dbgs() << "\n");
    Node->setNodeId(-1);
    return;
  }
  llvm::SDLoc DL(Node);
  SelectCode(Node);
}
