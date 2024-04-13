#include <M86.h>
#include <M86MachineFunctionInfo.h>
#include <cassert>
#include <llvm/CodeGen/MachineFunction.h>

using namespace llvm;

llvm::M86MachineFunctionInfo::M86MachineFunctionInfo() { M86_DEBUG_FUNCTION(); }

llvm::M86MachineFunctionInfo::M86MachineFunctionInfo(
    llvm::MachineFunction &MF) {
  M86_DEBUG_FUNCTION();
}

llvm::M86MachineFunctionInfo::~M86MachineFunctionInfo() {
  M86_DEBUG_FUNCTION();
}

void llvm::M86MachineFunctionInfo::setVarArgsFrameIndex(int Off) {
  M86_DEBUG_FUNCTION();

  VarArgsFrameIndex = Off;
}
int llvm::M86MachineFunctionInfo::getVarArgsFrameIndex() const {
  M86_DEBUG_FUNCTION();

  return VarArgsFrameIndex;
}

void llvm::M86MachineFunctionInfo::setVarArgsSaveSize(int Size) {
  M86_DEBUG_FUNCTION();

  VarArgsSaveSize = Size;
}
int llvm::M86MachineFunctionInfo::getVarArgsSaveSize() const {
  M86_DEBUG_FUNCTION();

  return VarArgsSaveSize;
}

unsigned llvm::M86MachineFunctionInfo::getCalleeSavedStackSize() const {
  M86_DEBUG_FUNCTION();

  return CalleeSavedStackSize;
}
void llvm::M86MachineFunctionInfo::setCalleeSavedStackSize(unsigned Size) {
  M86_DEBUG_FUNCTION();

  CalleeSavedStackSize = Size;
}

void llvm::M86MachineFunctionInfo::setReturnStackOffset(unsigned Off) {
  M86_DEBUG_FUNCTION();

  assert(!ReturnStackOffsetSet && "Return stack offset set twice");
  ReturnStackOffset = Off;
  ReturnStackOffsetSet = true;
}

unsigned llvm::M86MachineFunctionInfo::getReturnStackOffset() const {
  M86_DEBUG_FUNCTION();

  assert(ReturnStackOffsetSet && "Return stack offset not set");
  return ReturnStackOffset;
}

void llvm::M86MachineFunctionInfo::anchor() { M86_DEBUG_FUNCTION(); }
