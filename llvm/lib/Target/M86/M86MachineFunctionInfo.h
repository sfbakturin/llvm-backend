//===----------------------------------------------------------------------===//
//
// This file declares M86-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_M86_M86MACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_M86_M86MACHINEFUNCTIONINFO_H

#include <llvm/CodeGen/MachineFunction.h>

namespace llvm {

/// M86MachineFunctionInfo - This class is derived from MachineFunction private
/// M86 target-specific information for each MachineFunction.
class M86MachineFunctionInfo : public MachineFunctionInfo {
public:
  M86MachineFunctionInfo();
  explicit M86MachineFunctionInfo(MachineFunction &MF);
  ~M86MachineFunctionInfo();

  void setVarArgsFrameIndex(int Off);
  int getVarArgsFrameIndex() const;

  void setVarArgsSaveSize(int Size);
  int getVarArgsSaveSize() const;

  unsigned getCalleeSavedStackSize() const;
  void setCalleeSavedStackSize(unsigned Size);

  void setReturnStackOffset(unsigned Off);
  unsigned getReturnStackOffset() const;

  void setMaxCallStackReq(unsigned Req);
  unsigned getMaxCallStackReq() const;

private:
  //  virtual void anchor();

  bool ReturnStackOffsetSet = false;
  unsigned ReturnStackOffset = -1U;

  /// FrameIndex for start of varargs area
  int VarArgsFrameIndex = 0;
  /// Size of the save area used for varargs
  int VarArgsSaveSize = 0;
  /// Size of stack frame to save callee saved registers
  unsigned CalleeSavedStackSize = 0;

  unsigned MaxCallStackReq = 0;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86MACHINEFUNCTIONINFO_H
