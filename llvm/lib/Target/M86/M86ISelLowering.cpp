#include <M86.h>
#include <M86GenRegisterInfo.inc>
#include <M86ISelLowering.h>
#include <M86MachineFunctionInfo.h>
#include <M86Subtarget.h>
#include <cassert>
#include <cstdint>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/CodeGen/CallingConvLower.h>
#include <llvm/CodeGen/GenVT.inc>
#include <llvm/CodeGen/ISDOpcodes.h>
#include <llvm/CodeGen/MachineFrameInfo.h>
#include <llvm/CodeGen/MachineFunction.h>
#include <llvm/CodeGen/MachineMemOperand.h>
#include <llvm/CodeGen/MachineRegisterInfo.h>
#include <llvm/CodeGen/MachineValueType.h>
#include <llvm/CodeGen/Register.h>
#include <llvm/CodeGen/SelectionDAG.h>
#include <llvm/CodeGen/SelectionDAGNodes.h>
#include <llvm/CodeGen/TargetCallingConv.h>
#include <llvm/CodeGen/TargetLowering.h>
#include <llvm/CodeGen/TargetRegisterInfo.h>
#include <llvm/CodeGen/ValueTypes.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/IntrinsicsM86.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/MC/MCRegister.h>
#include <llvm/Support/Alignment.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/MathExtras.h>
#include <llvm/Support/TypeSize.h>
#include <llvm/Target/TargetMachine.h>
#include <utility>
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "M86-lower"

static const llvm::MCPhysReg ArgGPRs[] = {llvm::M86::RU1, llvm::M86::RU2,
                                          llvm::M86::RU3, llvm::M86::RU4};

void llvm::M86TargetLowering::ReplaceNodeResults(
    llvm::SDNode *N, llvm::SmallVectorImpl<llvm::SDValue> &Results,
    llvm::SelectionDAG &DAG) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  llvm_unreachable("");
}

llvm::M86TargetLowering::M86TargetLowering(const llvm::TargetMachine &TM,
                                           const llvm::M86Subtarget &STI)
    : llvm::TargetLowering(TM), STI(STI) {
  M86_START_FUNCTION();

  addRegisterClass(llvm::MVT::i32, &llvm::M86::GPRRegClass);

  computeRegisterProperties(STI.getRegisterInfo());

  setStackPointerRegisterToSaveRestore(llvm::M86::RS);

  for (unsigned Opc = 0; Opc < llvm::ISD::BUILTIN_OP_END; ++Opc) {
    setOperationAction(Opc, llvm::MVT::i32, Expand);
  }

  setOperationAction(llvm::ISD::ADD, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::MUL, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::SUB, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::SHL, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::SRL, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::AND, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::SRA, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::OR, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::XOR, llvm::MVT::i32, Legal);

  setOperationAction(llvm::ISD::LOAD, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::STORE, llvm::MVT::i32, Legal);

  setOperationAction(llvm::ISD::Constant, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::UNDEF, llvm::MVT::i32, Legal);

  setOperationAction(llvm::ISD::BR_CC, llvm::MVT::i32, Custom);

  setOperationAction(llvm::ISD::SELECT_CC, llvm::MVT::i32, Custom);
  setOperationAction(llvm::ISD::FRAMEADDR, llvm::MVT::i32, Legal);
  setOperationAction(llvm::ISD::INTRINSIC_VOID, llvm::MVT::i32, Custom);
  setOperationAction(llvm::ISD::SIGN_EXTEND_INREG, llvm::MVT::i1, Custom);

  M86_END_FUNCTION();
}

const char *llvm::M86TargetLowering::getTargetNodeName(unsigned Opcode) const {
  M86_START_FUNCTION();

  switch (Opcode) {
  case llvm::M86ISD::CALL:
    M86_END_FUNCTION();
    return "M86ISD::CALL";
  case llvm::M86ISD::RET:
    M86_END_FUNCTION();
    return "M86ISD::RET";
  case llvm::M86ISD::JCC:
    M86_END_FUNCTION();
    return "M86ISD::JCC";
  }

  M86_END_FUNCTION();
  return nullptr;
}

//===----------------------------------------------------------------------===//
//  Misc Lower Operation implementation
//===----------------------------------------------------------------------===//

#include <M86GenCallingConv.inc>

llvm::SDValue
llvm::M86TargetLowering::LowerOperation(llvm::SDValue Op,
                                        llvm::SelectionDAG &DAG) const {
  M86_START_FUNCTION();
  switch (Op->getOpcode()) {
  case llvm::ISD::BR_CC:
    M86_END_FUNCTION();
    return lowerJCC(Op, DAG);
  case llvm::ISD::FRAMEADDR:
    M86_END_FUNCTION();
    return lowerFIAddress(Op, DAG);
  case llvm::ISD::SELECT_CC:
    M86_END_FUNCTION();
    return lowerCondMOV(Op, DAG);
  case llvm::ISD::SELECT:
    M86_END_FUNCTION();
    return lowerSELECT(Op, DAG);
  case llvm::ISD::SIGN_EXTEND_INREG:
    M86_END_FUNCTION();
    return lowerSignExtendedInRegister(Op, DAG);
  default:
    M86_END_FUNCTION();
    llvm_unreachable("");
  }
}

//===----------------------------------------------------------------------===//
//                  Call Calling Convention Implementation
//===----------------------------------------------------------------------===//

static llvm::Align getPrefTypeAlign(llvm::EVT VT, llvm::SelectionDAG &DAG) {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return DAG.getDataLayout().getPrefTypeAlign(
      VT.getTypeForEVT(*DAG.getContext()));
}

llvm::SDValue llvm::M86TargetLowering::LowerCall(
    llvm::TargetLowering::CallLoweringInfo &CLI,
    llvm::SmallVectorImpl<llvm::SDValue> &InVals) const {
  M86_START_FUNCTION();

  llvm::SelectionDAG &DAG = CLI.DAG;
  llvm::SDLoc &DL = CLI.DL;
  llvm::SmallVectorImpl<llvm::ISD::OutputArg> &Outs = CLI.Outs;
  llvm::SmallVectorImpl<llvm::SDValue> &OutVals = CLI.OutVals;
  llvm::SmallVectorImpl<llvm::ISD::InputArg> &Ins = CLI.Ins;
  llvm::SDValue Chain = CLI.Chain;
  llvm::SDValue Callee = CLI.Callee;
  assert(!CLI.IsTailCall);
  llvm::CallingConv::ID CallConv = CLI.CallConv;
  bool IsVarArg = CLI.IsVarArg;
  llvm::EVT PtrVT = getPointerTy(DAG.getDataLayout());

  llvm::MachineFunction &MF = DAG.getMachineFunction();

  // Analyze the operands of the call, assigning locations to each operand.
  llvm::SmallVector<llvm::CCValAssign, 16> ArgLocs;
  llvm::CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeCallOperands(Outs, M86CCCall);

  // Get a count of how many bytes are to be pushed on the stack.
  unsigned NumBytes = CCInfo.getStackSize();

  // Create local copies for byval args
  llvm::SmallVector<llvm::SDValue, 8> ByValArgs;
  for (unsigned i = 0, e = Outs.size(); i != e; ++i) {
    llvm::ISD::ArgFlagsTy Flags = Outs[i].Flags;
    if (!Flags.isByVal())
      continue;

    llvm::SDValue Arg = OutVals[i];
    unsigned Size = Flags.getByValSize();
    llvm::Align Alignment = Flags.getNonZeroByValAlign();

    int FI =
        MF.getFrameInfo().CreateStackObject(Size, Alignment, /*isSS=*/false);
    llvm::SDValue FIPtr =
        DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
    llvm::SDValue SizeNode = DAG.getConstant(Size, DL, llvm::MVT::i32);

    Chain =
        DAG.getMemcpy(Chain, DL, FIPtr, Arg, SizeNode, Alignment,
                      /*IsVolatile=*/false,
                      /*AlwaysInline=*/false, false, llvm::MachinePointerInfo(),
                      llvm::MachinePointerInfo());
    ByValArgs.push_back(FIPtr);
  }

  Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0, CLI.DL);

  // Copy argument values to their designated locations.
  llvm::SmallVector<std::pair<llvm::Register, llvm::SDValue>, 8> RegsToPass;
  llvm::SmallVector<llvm::SDValue, 8> MemOpChains;
  llvm::SDValue StackPtr;
  for (unsigned i = 0, j = 0, e = ArgLocs.size(); i != e; ++i) {
    llvm::CCValAssign &VA = ArgLocs[i];
    llvm::SDValue ArgValue = OutVals[i];
    llvm::ISD::ArgFlagsTy Flags = Outs[i].Flags;
    // Promote the value if needed.
    // For now, only handle fully promoted and indirect arguments.
    if (VA.getLocInfo() == llvm::CCValAssign::Indirect) {
      // Store the argument in a stack slot and pass its address.
      llvm::Align StackAlign =
          std::max(getPrefTypeAlign(Outs[i].ArgVT, DAG),
                   getPrefTypeAlign(ArgValue.getValueType(), DAG));
      llvm::TypeSize StoredSize = ArgValue.getValueType().getStoreSize();
      unsigned ArgIndex = Outs[i].OrigArgIndex;
      unsigned ArgPartOffset = Outs[i].PartOffset;
      assert(ArgPartOffset == 0);
      // Calculate the total size to store. We don't have access to what we're
      // actually storing other than performing the loop and collecting the
      // info.
      llvm::SmallVector<std::pair<llvm::SDValue, llvm::SDValue>> Parts;
      while (i + 1 != e && Outs[i + 1].OrigArgIndex == ArgIndex) {
        llvm::SDValue PartValue = OutVals[i + 1];
        unsigned PartOffset = Outs[i + 1].PartOffset - ArgPartOffset;
        llvm::SDValue Offset = DAG.getIntPtrConstant(PartOffset, DL);
        llvm::EVT PartVT = PartValue.getValueType();
        StoredSize += PartVT.getStoreSize();
        StackAlign = std::max(StackAlign, getPrefTypeAlign(PartVT, DAG));
        Parts.push_back(std::make_pair(PartValue, Offset));
        ++i;
      }
      llvm::SDValue SpillSlot =
          DAG.CreateStackTemporary(StoredSize, StackAlign);
      int FI = llvm::cast<llvm::FrameIndexSDNode>(SpillSlot)->getIndex();
      MemOpChains.push_back(
          DAG.getStore(Chain, DL, ArgValue, SpillSlot,
                       llvm::MachinePointerInfo::getFixedStack(MF, FI)));
      for (const auto &Part : Parts) {
        llvm::SDValue PartValue = Part.first;
        llvm::SDValue PartOffset = Part.second;
        llvm::SDValue Address =
            DAG.getNode(llvm::ISD::ADD, DL, PtrVT, SpillSlot, PartOffset);
        MemOpChains.push_back(
            DAG.getStore(Chain, DL, PartValue, Address,
                         llvm::MachinePointerInfo::getFixedStack(MF, FI)));
      }
      ArgValue = SpillSlot;
    } else {
      assert(VA.getLocInfo() == llvm::CCValAssign::Full);
    }

    // Use local copy if it is a byval arg.
    if (Flags.isByVal())
      ArgValue = ByValArgs[j++];

    if (VA.isRegLoc()) {
      // Queue up the argument copies and emit them at the end.
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), ArgValue));
    } else {
      assert(VA.isMemLoc() && "Argument not register or memory");

      // Work out the address of the stack slot.
      if (!StackPtr.getNode())
        StackPtr = DAG.getCopyFromReg(Chain, DL, llvm::M86::RS, PtrVT);
      llvm::SDValue Address =
          DAG.getNode(llvm::ISD::ADD, DL, PtrVT, StackPtr,
                      DAG.getIntPtrConstant(VA.getLocMemOffset(), DL));

      // Emit the store.
      MemOpChains.push_back(DAG.getStore(Chain, DL, ArgValue, Address,
                                         llvm::MachinePointerInfo()));
    }
  }

  // Join the stores, which are independent of one another.
  if (!MemOpChains.empty())
    Chain =
        DAG.getNode(llvm::ISD::TokenFactor, DL, llvm::MVT::Other, MemOpChains);

  llvm::SDValue Glue;

  // Build a sequence of copy-to-reg nodes, chained and glued together.
  for (auto &Reg : RegsToPass) {
    Chain = DAG.getCopyToReg(Chain, DL, Reg.first, Reg.second, Glue);
    Glue = Chain.getValue(1);
  }

  // No external symbols support
  if (llvm::GlobalAddressSDNode *S =
          llvm::dyn_cast<llvm::GlobalAddressSDNode>(Callee)) {
    const llvm::GlobalValue *GV = S->getGlobal();
    assert(getTargetMachine().shouldAssumeDSOLocal(*GV->getParent(), GV));
    Callee = DAG.getTargetGlobalAddress(GV, DL, PtrVT, 0, 0);
  }

  // The first call operand is the chain and the second is the target address.
  llvm::SmallVector<llvm::SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  // Add argument registers to the end of the list so that they are
  // known live into the call.
  for (auto &Reg : RegsToPass)
    Ops.push_back(DAG.getRegister(Reg.first, Reg.second.getValueType()));

  // Add a register mask operand representing the call-preserved registers.
  const llvm::TargetRegisterInfo *TRI = STI.getRegisterInfo();
  const std::uint32_t *Mask = TRI->getCallPreservedMask(MF, CallConv);
  Ops.push_back(DAG.getRegisterMask(Mask));

  // Glue the call to the argument copies, if any.
  if (Glue.getNode())
    Ops.push_back(Glue);

  // Emit the call.
  llvm::SDVTList NodeTys = DAG.getVTList(llvm::MVT::Other, llvm::MVT::Glue);

  Chain = DAG.getNode(llvm::M86ISD::CALL, DL, NodeTys, Ops);
  DAG.addNoMergeSiteInfo(Chain.getNode(), CLI.NoMerge);
  Glue = Chain.getValue(1);

  // Mark the end of the call, which is glued to the call itself.
  Chain = DAG.getCALLSEQ_END(Chain, DAG.getConstant(NumBytes, DL, PtrVT, true),
                             DAG.getConstant(0, DL, PtrVT, true), Glue, DL);
  Glue = Chain.getValue(1);

  // Assign locations to each value returned by this call.
  llvm::SmallVector<llvm::CCValAssign, 16> RVLocs;
  llvm::CCState RetCCInfo(CallConv, IsVarArg, MF, RVLocs, *DAG.getContext());
  RetCCInfo.AnalyzeCallResult(Ins, M86CCRet);

  // Copy all of the result registers out of their specified physreg.
  for (auto &VA : RVLocs) {
    // Copy the value out
    llvm::SDValue RetValue =
        DAG.getCopyFromReg(Chain, DL, VA.getLocReg(), VA.getLocVT(), Glue);
    // Glue the RetValue to the end of the call sequence
    Chain = RetValue.getValue(1);
    Glue = RetValue.getValue(2);

    assert(VA.getLocInfo() == llvm::CCValAssign::Full);
    InVals.push_back(RetValue);
  }

  M86_END_FUNCTION();

  return Chain;
}

//===----------------------------------------------------------------------===//
//             Formal Arguments Calling Convention Implementation
//===----------------------------------------------------------------------===//

namespace {

struct ArgDataPair {
  llvm::SDValue SDV;
  llvm::ISD::ArgFlagsTy Flags;
};

} // end anonymous namespace

static llvm::SDValue convertValVTToLocVT(llvm::SelectionDAG &DAG,
                                         llvm::SDValue Val,
                                         const llvm::CCValAssign &VA,
                                         const llvm::SDLoc &DL,
                                         const llvm::M86Subtarget &Subtarget) {
  M86_START_FUNCTION();

  llvm::EVT LocVT = VA.getLocVT();

  if (VA.getValVT() == llvm::MVT::f32)
    llvm_unreachable("");

  switch (VA.getLocInfo()) {
  default:
    llvm_unreachable("Unexpected CCValAssign::LocInfo");
  case llvm::CCValAssign::Full:
    break;
  case llvm::CCValAssign::BCvt:
    llvm_unreachable("");
    Val = DAG.getNode(llvm::ISD::BITCAST, DL, LocVT, Val);
    break;
  }

  M86_END_FUNCTION();

  return Val;
}

// Convert Val to a ValVT. Should not be called for CCValAssign::Indirect
// values.
static llvm::SDValue convertLocVTToValVT(llvm::SelectionDAG &DAG,
                                         llvm::SDValue Val,
                                         const llvm::CCValAssign &VA,
                                         const llvm::SDLoc &DL,
                                         const llvm::M86Subtarget &Subtarget) {
  M86_START_FUNCTION();

  if (VA.getValVT() == llvm::MVT::f32)
    llvm_unreachable("");

  switch (VA.getLocInfo()) {
  default:
    llvm_unreachable("Unexpected CCValAssign::LocInfo");
  case llvm::CCValAssign::Full:
    break;
  case llvm::CCValAssign::BCvt:
    llvm_unreachable("");
    Val = DAG.getNode(llvm::ISD::BITCAST, DL, VA.getValVT(), Val);
  }

  M86_END_FUNCTION();

  return Val;
}

static llvm::SDValue unpackFromRegLoc(llvm::SelectionDAG &DAG,
                                      llvm::SDValue Chain,
                                      const llvm::CCValAssign &VA,
                                      const llvm::SDLoc &DL,
                                      const llvm::M86TargetLowering &TLI) {
  M86_START_FUNCTION();

  llvm::MachineFunction &MF = DAG.getMachineFunction();
  llvm::MachineRegisterInfo &RegInfo = MF.getRegInfo();
  llvm::EVT LocVT = VA.getLocVT();
  llvm::SDValue Val;
  const llvm::TargetRegisterClass *RC = TLI.getRegClassFor(LocVT.getSimpleVT());
  llvm::Register VReg = RegInfo.createVirtualRegister(RC);
  RegInfo.addLiveIn(VA.getLocReg(), VReg);
  Val = DAG.getCopyFromReg(Chain, DL, VReg, LocVT);

  if (VA.getLocInfo() == llvm::CCValAssign::Indirect) {
    M86_END_FUNCTION();
    return Val;
  }

  M86_END_FUNCTION();

  return convertLocVTToValVT(DAG, Val, VA, DL, TLI.getSubtarget());
}

// The caller is responsible for loading the full value if the argument is
// passed with CCValAssign::Indirect.
static llvm::SDValue unpackFromMemLoc(llvm::SelectionDAG &DAG,
                                      llvm::SDValue Chain,
                                      const llvm::CCValAssign &VA,
                                      const llvm::SDLoc &DL) {
  M86_START_FUNCTION();

  llvm::MachineFunction &MF = DAG.getMachineFunction();
  llvm::MachineFrameInfo &MFI = MF.getFrameInfo();
  llvm::EVT LocVT = VA.getLocVT();
  llvm::EVT ValVT = VA.getValVT();
  llvm::EVT PtrVT =
      llvm::MVT::getIntegerVT(DAG.getDataLayout().getPointerSizeInBits(0));
  int FI = MFI.CreateFixedObject(ValVT.getStoreSize(), VA.getLocMemOffset(),
                                 /*IsImmutable=*/true);
  llvm::SDValue FIN = DAG.getFrameIndex(FI, PtrVT);
  llvm::SDValue Val;

  llvm::ISD::LoadExtType ExtType;
  switch (VA.getLocInfo()) {
  default:
    llvm_unreachable("Unexpected CCValAssign::LocInfo");
  case llvm::CCValAssign::Full:
  case llvm::CCValAssign::Indirect:
  case llvm::CCValAssign::BCvt:
    ExtType = llvm::ISD::NON_EXTLOAD;
    break;
  }
  Val = DAG.getExtLoad(
      ExtType, DL, LocVT, Chain, FIN,
      llvm::MachinePointerInfo::getFixedStack(DAG.getMachineFunction(), FI),
      ValVT);

  M86_END_FUNCTION();

  return Val;
}

/// M86 formal arguments implementation
llvm::SDValue llvm::M86TargetLowering::LowerFormalArguments(
    llvm::SDValue Chain, llvm::CallingConv::ID CallConv, bool IsVarArg,
    const llvm::SmallVectorImpl<llvm::ISD::InputArg> &Ins,
    const llvm::SDLoc &DL, llvm::SelectionDAG &DAG,
    llvm::SmallVectorImpl<llvm::SDValue> &InVals) const {
  M86_START_FUNCTION();

  switch (CallConv) {
  default:
    report_fatal_error("Unsupported calling convention");
  case llvm::CallingConv::C:
  case llvm::CallingConv::Fast:
    break;
  }

  llvm::MachineFunction &MF = DAG.getMachineFunction();
  llvm::EVT PtrVT = getPointerTy(DAG.getDataLayout());
  unsigned StackSlotSize = llvm::MVT(llvm::MVT::i32).getSizeInBits() / 8;
  // Used with vargs to acumulate store chains.
  std::vector<llvm::SDValue> OutChains;

  // Assign locations to all of the incoming arguments.
  llvm::SmallVector<llvm::CCValAssign, 16> ArgLocs;
  llvm::CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, M86CCCall);

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    llvm::CCValAssign &VA = ArgLocs[i];
    llvm::SDValue ArgValue;
    if (VA.isRegLoc())
      ArgValue = unpackFromRegLoc(DAG, Chain, VA, DL, *this);
    else
      ArgValue = unpackFromMemLoc(DAG, Chain, VA, DL);

    if (VA.getLocInfo() == llvm::CCValAssign::Indirect) {
      InVals.push_back(DAG.getLoad(VA.getValVT(), DL, Chain, ArgValue,
                                   llvm::MachinePointerInfo()));
      unsigned ArgIndex = Ins[i].OrigArgIndex;
      unsigned ArgPartOffset = Ins[i].PartOffset;
      assert(ArgPartOffset == 0);
      while (i + 1 != e && Ins[i + 1].OrigArgIndex == ArgIndex) {
        llvm::CCValAssign &PartVA = ArgLocs[i + 1];
        unsigned PartOffset = Ins[i + 1].PartOffset - ArgPartOffset;
        llvm::SDValue Offset = DAG.getIntPtrConstant(PartOffset, DL);
        llvm::SDValue Address =
            DAG.getNode(llvm::ISD::ADD, DL, PtrVT, ArgValue, Offset);
        InVals.push_back(DAG.getLoad(PartVA.getValVT(), DL, Chain, Address,
                                     llvm::MachinePointerInfo()));
        ++i;
      }
      continue;
    }
    InVals.push_back(ArgValue);
  }

  if (IsVarArg) {
    llvm::ArrayRef<llvm::MCPhysReg> ArgRegs = llvm::ArrayRef(ArgGPRs);
    unsigned Idx = CCInfo.getFirstUnallocated(ArgRegs);
    const llvm::TargetRegisterClass *RC = &llvm::M86::GPRRegClass;
    llvm::MachineFrameInfo &MFI = MF.getFrameInfo();
    llvm::MachineRegisterInfo &RegInfo = MF.getRegInfo();
    llvm::M86MachineFunctionInfo *UFI =
        MF.getInfo<llvm::M86MachineFunctionInfo>();

    int VaArgOffset, VarArgsSaveSize;

    // If all registers are allocated, then all varargs must be passed on
    //    the
    // stack and we don't need to save any argregs.
    if (ArgRegs.size() == Idx) {
      VaArgOffset = CCInfo.getStackSize();
      VarArgsSaveSize = 0;
    } else {
      VarArgsSaveSize = StackSlotSize * (ArgRegs.size() - Idx);
      VaArgOffset = -VarArgsSaveSize;
    }

    // Record the frame index of the first variable argument
    // which is a value necessary to VASTART.
    int FI = MFI.CreateFixedObject(StackSlotSize, VaArgOffset, true);
    UFI->setVarArgsFrameIndex(FI);

    // If saving an odd number of registers then create an extra stack slot
    //    to
    // ensure that the frame pointer is 2*XLEN-aligned, which in turn
    //    ensures
    // offsets to even-numbered registered remain 2*XLEN-aligned.
    if (Idx % 2) {
      MFI.CreateFixedObject(StackSlotSize, VaArgOffset - (int)StackSlotSize,
                            true);
      VarArgsSaveSize += StackSlotSize;
    }

    // Copy the integer registers that may have been used for passing
    //    varargs
    // to the vararg save area.
    for (unsigned I = Idx; I < ArgRegs.size();
         ++I, VaArgOffset += StackSlotSize) {
      const llvm::Register Reg = RegInfo.createVirtualRegister(RC);
      RegInfo.addLiveIn(ArgRegs[I], Reg);
      llvm::SDValue ArgValue =
          DAG.getCopyFromReg(Chain, DL, Reg, llvm::MVT::i32);
      FI = MFI.CreateFixedObject(StackSlotSize, VaArgOffset, true);
      llvm::SDValue PtrOff =
          DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
      llvm::SDValue Store =
          DAG.getStore(Chain, DL, ArgValue, PtrOff,
                       llvm::MachinePointerInfo::getFixedStack(MF, FI));
      cast<llvm::StoreSDNode>(Store.getNode())
          ->getMemOperand()
          ->setValue((llvm::Value *)nullptr);
      OutChains.push_back(Store);
    }
    UFI->setVarArgsSaveSize(VarArgsSaveSize);
  }

  // All stores are grouped in one node to allow the matching between
  // the size of Ins and InVals. This only happens for vararg functions.
  if (!OutChains.empty()) {
    OutChains.push_back(Chain);
    Chain =
        DAG.getNode(llvm::ISD::TokenFactor, DL, llvm::MVT::Other, OutChains);
  }

  return Chain;

  M86_END_FUNCTION();
}

//===----------------------------------------------------------------------===//
//               Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//

bool llvm::M86TargetLowering::CanLowerReturn(
    llvm::CallingConv::ID CallConv, llvm::MachineFunction &MF, bool IsVarArg,
    const llvm::SmallVectorImpl<llvm::ISD::OutputArg> &Outs,
    llvm::LLVMContext &Context) const {
  M86_START_FUNCTION();

  llvm::SmallVector<llvm::CCValAssign, 16> RVLocs;
  llvm::CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, Context);
  if (!CCInfo.CheckReturn(Outs, M86CCRet)) {
    M86_END_FUNCTION();
    return false;
  }
  if (CCInfo.getStackSize() != 0 && IsVarArg)
    llvm_unreachable("");

  M86_END_FUNCTION();

  return true;
}

llvm::SDValue llvm::M86TargetLowering::LowerReturn(
    llvm::SDValue Chain, llvm::CallingConv::ID CallConv, bool IsVarArg,
    const llvm::SmallVectorImpl<llvm::ISD::OutputArg> &Outs,
    const llvm::SmallVectorImpl<llvm::SDValue> &OutVals, const llvm::SDLoc &DL,
    llvm::SelectionDAG &DAG) const {
  M86_START_FUNCTION();

  const llvm::MachineFunction &MF = DAG.getMachineFunction();
  const llvm::M86Subtarget &STI = MF.getSubtarget<llvm::M86Subtarget>();

  // Stores the assignment of the return value to a location.
  llvm::SmallVector<llvm::CCValAssign, 16> RVLocs;

  // Info about the registers and stack slot.
  llvm::CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                       *DAG.getContext());

  CCInfo.AnalyzeReturn(Outs, M86CCRet);

  llvm::SDValue Glue;
  llvm::SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    llvm::SDValue Val = OutVals[i];
    llvm::CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Val = convertValVTToLocVT(DAG, Val, VA, DL, STI);
    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Glue);

    // Guarantee that all emitted copies are stuck together.
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain; // Update chain.
  // Add the glue node if we have it.
  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  M86_END_FUNCTION();

  return DAG.getNode(llvm::M86ISD::RET, DL, llvm::MVT::Other, RetOps);
}

//===----------------------------------------------------------------------===//
// Target Optimization Hooks
//===----------------------------------------------------------------------===//

llvm::SDValue
llvm::M86TargetLowering::PerformDAGCombine(llvm::SDNode *N,
                                           DAGCombinerInfo &DCI) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return {};
}

//===----------------------------------------------------------------------===//
//  Addressing mode description hooks
//===----------------------------------------------------------------------===//

/// Return true if the addressing mode represented by AM is legal for this
/// target, for a load/store of the specified type.
bool llvm::M86TargetLowering::isLegalAddressingMode(
    const llvm::DataLayout &DL, const AddrMode &AM, llvm::Type *Ty, unsigned AS,
    llvm::Instruction *I) const {
  M86_START_FUNCTION();

  // No global is ever allowed as a base.
  if (AM.BaseGV) {
    M86_END_FUNCTION();
    return false;
  }

  if (!isInt<16>(AM.BaseOffs)) {
    M86_END_FUNCTION();
    return false;
  }

  switch (AM.Scale) {
  case 0: // "r+i" or just "i", depending on HasBaseReg.
    break;
  case 1:
    if (!AM.HasBaseReg) // allow "r+i".
      break;
    M86_END_FUNCTION();
    return false; // disallow "r+r" or "r+r+i".
  default:
    M86_END_FUNCTION();
    return false;
  }

  M86_END_FUNCTION();

  return true;
}

bool llvm::M86TargetLowering::mayBeEmittedAsTailCall(
    const llvm::CallInst *CI) const {
  M86_START_FUNCTION();
  M86_END_FUNCTION();
  return false;
}

static void translateSetCCForBranch(const llvm::SDLoc &DL, llvm::SDValue &LHS,
                                    llvm::SDValue &RHS, llvm::ISD::CondCode &CC,
                                    llvm::SelectionDAG &DAG) {
  M86_START_FUNCTION();
  switch (CC) {
  default:
    break;
  case llvm::ISD::SETLT:
  case llvm::ISD::SETGE:
    CC = llvm::ISD::getSetCCSwappedOperands(CC);
    std::swap(LHS, RHS);
    break;
  }
  M86_END_FUNCTION();
}

llvm::SDValue
llvm::M86TargetLowering ::lowerJCC(llvm::SDValue Op,
                                   llvm::SelectionDAG &DAG) const {
  M86_START_FUNCTION();
  llvm::SDValue CC = Op.getOperand(1);
  llvm::SDValue LHS = Op.getOperand(2);
  llvm::SDValue RHS = Op.getOperand(3);
  llvm::SDValue Block = Op->getOperand(4);
  llvm::SDLoc DL(Op);

  assert(LHS.getValueType() == llvm::MVT::i32);

  llvm::ISD::CondCode CCVal = llvm::cast<llvm::CondCodeSDNode>(CC)->get();
  translateSetCCForBranch(DL, LHS, RHS, CCVal, DAG);
  llvm::SDValue TargetCC = DAG.getCondCode(CCVal);
  M86_END_FUNCTION();
  return DAG.getNode(llvm::M86ISD::JCC, DL, Op.getValueType(), Op.getOperand(0),
                     LHS, RHS, TargetCC, Block);
}

llvm::SDValue
llvm::M86TargetLowering ::lowerFIAddress(llvm::SDValue Op,
                                         llvm::SelectionDAG &DAG) const {
  M86_START_FUNCTION();
  const llvm::M86RegisterInfo &RI = *STI.getRegisterInfo();
  llvm::MachineFunction &MF = DAG.getMachineFunction();
  llvm::MachineFrameInfo &MFI = MF.getFrameInfo();
  MFI.setFrameAddressIsTaken(true);
  llvm::Register FrameReg = RI.getFrameRegister(MF);
  llvm::EVT VT = Op.getValueType();
  llvm::SDLoc DL(Op);
  llvm::SDValue FrameAddr =
      DAG.getCopyFromReg(DAG.getEntryNode(), DL, FrameReg, VT);
  // Only for current frame
  assert(llvm::cast<llvm::ConstantSDNode>(Op.getOperand(0))->getZExtValue() ==
         0);
  M86_END_FUNCTION();
  return FrameAddr;
}

llvm::SDValue
llvm::M86TargetLowering::lowerCondMOV(llvm::SDValue Op,
                                      llvm::SelectionDAG &DAG) const {
  M86_START_FUNCTION();
  llvm::SDValue LHS = Op.getOperand(0);
  llvm::SDValue RHS = Op.getOperand(1);
  llvm::ISD::CondCode CC =
      llvm::cast<llvm::CondCodeSDNode>(Op.getOperand(4))->get();
  llvm::SDValue TVal = Op.getOperand(2);
  llvm::SDValue FVal = Op.getOperand(3);
  llvm::SDLoc dl(Op);
  translateSetCCForBranch(dl, LHS, RHS, CC, DAG);
  llvm::SDValue TargetCC = DAG.getCondCode(CC);
  M86_END_FUNCTION();
  return DAG.getNode(llvm::M86ISD::CMOV, dl, TVal.getValueType(), TVal, FVal,
                     TargetCC, LHS, RHS);
}

llvm::SDValue
llvm::M86TargetLowering::lowerSELECT(llvm::SDValue Op,
                                     llvm::SelectionDAG &DAG) const {
  return Op;
}

llvm::SDValue llvm::M86TargetLowering::lowerSignExtendedInRegister(
    llvm::SDValue Op, llvm::SelectionDAG &DAG) const {
  llvm::SDValue Op0 = Op.getOperand(0);
  llvm::SDLoc dl(Op);
  assert(Op.getValueType() == llvm::MVT::i32 &&
         "Unhandled target sign_extend_inreg.");
  // These are legal
  unsigned Width =
      cast<llvm::VTSDNode>(Op.getOperand(1))->getVT().getSizeInBits();
  if (Width == 16 || Width == 8)
    return Op;
  if (Width >= 32) {
    return {};
  }
  llvm::SDValue LS =
      DAG.getNode(llvm::ISD::SHL, dl, llvm::MVT::i32, Op0,
                  DAG.getConstant(32 - Width, dl, llvm::MVT::i32));
  llvm::SDValue SR =
      DAG.getNode(llvm::ISD::SRA, dl, llvm::MVT::i32, LS,
                  DAG.getConstant(32 - Width, dl, llvm::MVT::i32));
  return SR;
}
