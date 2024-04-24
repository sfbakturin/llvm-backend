//===----------------------------------------------------------------------===//
// M86 Return Value Calling Convention
//===----------------------------------------------------------------------===//

def M86CCRet : CallingConv<[
  // i32 return value
  CCIfType<[i32, i64], CCAssignToReg<[R13]>>,

  // Integer values get stored in stack slots that are 4 bytes in
  // size and 4-byte aligned.
  CCIfType<[i64], CCAssignToStack<8, 4>>,
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

//===----------------------------------------------------------------------===//
// M86 Argument Calling Conventions
//===----------------------------------------------------------------------===//

def M86CCCall : CallingConv<[
  // Promote i8/i16 arguments to i32.
  CCIfType<[i8, i16], CCPromoteToType<i32>>,
  
  // The first 4 integer arguments are passed in integer registers.
  CCIfType<[i32, i64], CCAssignToReg<[R3, R4, R5, R6]>>,

  // Integer values get stored in stack slots that are 4 bytes in
  // size and 4-byte aligned.
  CCIfType<[i64], CCAssignToStack<8, 4>>,
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

def CSR_M86 : CalleeSavedRegs<(add R0, R1, R11, R12, R14, R15)>;
