#include <M86.h>
#include <M86MachineFunctionInfo.h>
#include <cassert>
#include <llvm/CodeGen/MachineFunction.h>

using namespace llvm;

llvm::M86MachineFunctionInfo::M86MachineFunctionInfo() {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

llvm::M86MachineFunctionInfo::M86MachineFunctionInfo(
    llvm::MachineFunction &MF) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

llvm::M86MachineFunctionInfo::~M86MachineFunctionInfo() {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
}

void llvm::M86MachineFunctionInfo::setVarArgsFrameIndex(int Off) {
  M86_START_FUNCTION();
  VarArgsFrameIndex = Off;
  M86_END_FUNCTION();
}

int llvm::M86MachineFunctionInfo::getVarArgsFrameIndex() const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return VarArgsFrameIndex;
}

void llvm::M86MachineFunctionInfo::setVarArgsSaveSize(int Size) {
  M86_START_FUNCTION();
  VarArgsSaveSize = Size;
  M86_END_FUNCTION();
}
int llvm::M86MachineFunctionInfo::getVarArgsSaveSize() const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return VarArgsSaveSize;
}

unsigned llvm::M86MachineFunctionInfo::getCalleeSavedStackSize() const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return CalleeSavedStackSize;
}

void llvm::M86MachineFunctionInfo::setCalleeSavedStackSize(unsigned Size) {
  M86_START_FUNCTION();
  CalleeSavedStackSize = Size;
  M86_END_FUNCTION();
}

void llvm::M86MachineFunctionInfo::setReturnStackOffset(unsigned Off) {
  M86_START_FUNCTION();
  assert(!ReturnStackOffsetSet && "Return stack offset set twice");
  ReturnStackOffset = Off;
  ReturnStackOffsetSet = true;
  M86_END_FUNCTION();
}

unsigned llvm::M86MachineFunctionInfo::getReturnStackOffset() const {
  M86_START_FUNCTION();
  assert(ReturnStackOffsetSet && "Return stack offset not set");
  M86_END_FUNCTION();
  return ReturnStackOffset;
}

void llvm::M86MachineFunctionInfo::setMaxCallStackReq(unsigned Req) {
  M86_START_FUNCTION();
  MaxCallStackReq = Req;
  M86_END_FUNCTION();
}

unsigned llvm::M86MachineFunctionInfo::getMaxCallStackReq() const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return MaxCallStackReq;
}
