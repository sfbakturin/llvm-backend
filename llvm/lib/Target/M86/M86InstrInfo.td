//===----------------------------------------------------------------------===//
// M86 instruction's format.
//===----------------------------------------------------------------------===//

include "M86InstrFormats.td"

//===----------------------------------------------------------------------===//
// M86 profiles and nodes.
//===----------------------------------------------------------------------===//

def M86SDTCall         : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;
def M86SDTCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def M86SDTCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def M86SDTJCC          : SDTypeProfile<0, 4, [SDTCisSameAs<0, 1>, SDTCisVT<2, OtherVT>, SDTCisVT<3, OtherVT>]>;
def M86SDTCMOV         : SDTypeProfile<1, 5, [SDTCisSameAs<0, 1>, SDTCisSameAs<1, 2>, SDTCisVT<3, OtherVT>, SDTCisVT<4, i32>,SDTCisVT<4, i32> ]>;

def m86_call          : SDNode<"M86ISD::CALL",       M86SDTCall,         [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue, SDNPVariadic]>;
def m86_callseq_end   : SDNode<"ISD::CALLSEQ_START", M86SDTCallSeqStart, [SDNPHasChain, SDNPOutGlue]>;
def m86_callseq_start : SDNode<"ISD::CALLSEQ_END",   M86SDTCallSeqEnd,   [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def m86_ret           : SDNode<"M86ISD::RET",        SDTNone,            [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def m86_jcc           : SDNode<"M86ISD::JCC",        M86SDTJCC,          [SDNPHasChain]>;
def m86_cmov          : SDNode<"M86ISD::CMOV",       M86SDTCMOV,         [SDNPMayStore, SDNPMemOperand]>;

//===----------------------------------------------------------------------===//
// M86 specific microcode.
//===----------------------------------------------------------------------===//

let Defs = [R11], Uses = [R11] in {
	def ADJCALLSTACKDOWN : M86Microcode<
		(outs),
		(ins i32imm:$amt, i32imm:$amt2),
		"# ADJCALLSTACKDOWN $amt, $amt2",
		[(m86_callseq_start timm:$amt, timm:$amt2)]
	>;
	def ADJCALLSTACKUP   : M86Microcode<
		(outs),
		(ins i32imm:$amt1, i32imm:$amt2),
		"# ADJCALLSTACKUP $amt1",
		[(m86_callseq_end timm:$amt1, timm:$amt2)]
	>;
}

def IsOrAdd: PatFrag<(ops node:$A, node:$B), (or node:$A, node:$B), [{return isOrEquivalentToAdd(N);}]>;

//===----------------------------------------------------------------------===//
// M86 operands.
//===----------------------------------------------------------------------===//

class ImmAsmOperand<string prefix, int width> : AsmOperandClass {
  let Name = "M86" # prefix # "Imm" # width;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

class SignedImmAsmOperand<int width> : ImmAsmOperand<"Signed", width>;
class UnsignedImmAsmOperand<int width> : ImmAsmOperand<"Unsigned", width>;

def m86_simm : Operand<i32>, ImmLeaf<i32, [{ return isInt<16>(Imm); }]>
{
	let ParserMatchClass = SignedImmAsmOperand<16>;
	let EncoderMethod = "getImmOpValue";
	let DecoderMethod = "decodeSignedImmOperand<16>";
	let MCOperandPredicate =
	[{
			int64_t Imm;
			if (MCOp.evaluateAsConstantImm(Imm))
			{
				return isInt<16>(Imm);
			}
			return MCOp.isBareSymbolRef();
	}];
	let OperandType = "OPERAND_SIGNED_IMM";
	let OperandNamespace = "M86Op";
}

def m86_uimm : Operand<i32>, ImmLeaf<i32, [{ return isUInt<16>(Imm); }]>
{
	let ParserMatchClass = UnsignedImmAsmOperand<16>;
	let EncoderMethod = "getImmOpValue";
	let DecoderMethod = "decodeUnsignedImmOperand<16>";
	let MCOperandPredicate =
	[{
			int64_t Imm;
			if (MCOp.evaluateAsConstantImm(Imm))
			{
				return isUInt<16>(Imm);
			}
			return MCOp.isBareSymbolRef();
	}];
	let OperandType = "OPERAND_UNSIGNED_IMM";
	let OperandNamespace = "M86Op";
}

def m86_btarget : Operand<OtherVT>
{
	let ParserMatchClass = SignedImmAsmOperand<16>;
	let PrintMethod = "printBranchOperand";
	let EncoderMethod = "getBranchTargetOpValue";
	let DecoderMethod = "decodeSignedImmOperand<16>";
	let MCOperandPredicate =
	[{
		int64_t Imm;
		if (MCOp.evaluateAsConstantImm(Imm))
		{
			return isInt<16>(Imm);
		}
		return MCOp.isBareSymbolRef();
	}];
	let OperandType = "OPERAND_PCREL";
}

def m86_ctarget : Operand<i32> {
	let ParserMatchClass = SignedImmAsmOperand<16>;
	let PrintMethod = "printBranchOperand";
	let EncoderMethod = "getBranchTargetOpValue";
	let DecoderMethod = "decodeSignedImmOperand<16>";
	let MCOperandPredicate =
	[{
		int64_t Imm;
		if (MCOp.evaluateAsConstantImm(Imm))
		{
			return isInt<16>(Imm);
		}
		return MCOp.isBareSymbolRef();
	}];
	let OperandType = "OPERAND_PCREL";
}

def m86_immli : SDNodeXForm<imm, [{return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0xFFFF, SDLoc(N), MVT::i32);}]>;
def m86_immhi : SDNodeXForm<imm, [{return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() >> 16, SDLoc(N), MVT::i32);}]>;

def m86_i32li16 : Operand<i32>, PatLeaf<(i32 imm), [{ return ((N->getZExtValue() & 0xFFFFUL) == N->getZExtValue()); }], m86_immli>;
def m86_i32hi16 : Operand<i32>, PatLeaf<(i32 imm), [{ return ((N->getZExtValue() & 0xFFFF0000UL) == N->getZExtValue());}], m86_immhi>;

// Addressing modes
def m86_fiaddr   : ComplexPattern<iPTR, 1, "SelectFIAddress", [frameindex], []>;
def m86_baseaddr : ComplexPattern<iPTR, 1, "SelectBaseAddress">;

class M86PatternJCC<CondCode Cond, M86Instruction Inst> :
Pat
<
	(m86_jcc GPR:$Dst, GPR:$Src, Cond, bb:$Acc),
	(Inst GPR:$Dst, GPR:$Src, m86_btarget:$Acc)
>;

//===----------------------------------------------------------------------===//
// M86 instructions specific formats.
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBranch = 1, isTerminator = 1, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
class M86TemplateJCC<bits<8> op, string opcodestr> : M86Instruction<
	op,
	(outs),
	(ins GPR:$Dst, GPR:$Src, m86_btarget:$Acc),
	!strconcat(opcodestr, " $Dst $Src $Acc"),
	[]
>;

//===----------------------------------------------------------------------===//
// M86 instructions.
//===----------------------------------------------------------------------===//

// Conditional Move.

//// CMOVEQ - Conditional MOVe when EQuals.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVEQ : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVEQ $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETEQ, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVGT - Conditional MOVe when Greater Than.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVGT : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVGT $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETGT, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVGE - Conditional MOVe when Greater than or Equals.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVGE : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVGE $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETGE, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVLT - Conditional MOVe when Less Than.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVLT : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVLT $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETLT, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVLE - Conditional MOVe when Less than or Equals.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVLE : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVLE $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETLE, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVNE - Conditional MOVe when Not Equals.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVNE : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVNE $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETNE, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVUEQ - Conditional MOVe when EQuals as Unsigned.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVUEQ : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVUEQ $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETUEQ, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVUGT - Conditional MOVe when Greater Than as Unsigned.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVUGT : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVUGT $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETUGT, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVUGE - Conditional MOVe when Greater than or Equals as Unsigned.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVUGE : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVUGE $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETUGE, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVULT - Conditional MOVe when Less Than as Unsigned.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVULT : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVULT $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETULT, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVULE - Conditional MOVe when Less than or Equals as Unsigned.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVULE : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVULE $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETULE, GPR:$Acc, GPR:$AR1))]
>;

//// CMOVUNE - Conditional MOVe when Not Equals as Unsigned.
let Constraints = "$Dst_out = $Dst", AR2 = 0, AR3 = 0, AR4 = 0 in
def CMOVUNE : M86Instruction<
	1,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc, GPR:$AR1),
	"CMOVUNE $Dst $Src $Acc $AR1",
	[(set GPR:$Dst_out, (m86_cmov GPR:$Dst, GPR:$Src, SETUNE, GPR:$Acc, GPR:$AR1))]
>;

// Graphical.

//// PP - Put Pixel.
let AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def PP : M86Instruction<
	1,
	(outs),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc),
	"PP $Dst $Src $Acc",
	[(int_m86_putpixel GPR:$Dst, GPR:$Src, GPR:$Acc)]
>;

//// FLS - FLuSh to screen.
let Dst = 0, Src = 0, Acc = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def FLS : M86Instruction<
	2,
	(outs),
	(ins),
	"FLS",
	[(int_m86_flush)]
>;

//// RND - RaNDom number.
let Src = 0, Acc = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def RND : M86Instruction<
	3,
	(outs GPR:$Dst),
	(ins),
	"RND $Dst",
	[(set GPR:$Dst, (int_m86_simrand))]
>;

//// SELECT - SELECT value from truefy falsefy, if cond.
let AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def SELECT : M86Instruction<
	3,
	(outs GPR:$R2),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc),
	"SELECT $Dst $Src $Acc",
	[(set GPR:$R2, (int_m86_select GPR:$Dst, GPR:$Src, GPR:$Acc))]
>;

// Control flow.

//// JMPL - JuMP to Label, unconditional jump by offset.
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBarrier = 1, isBranch = 1, isTerminator = 1, Dst = 0, Src = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def JMPL : M86Instruction<
	0x00,
	(outs),
	(ins m86_btarget:$Acc),
	"JMPL $Acc",
	[(br bb:$Acc)]
>;

//// JMPA - JuMP to Address, unconditional jump to address.
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1, Src = 0, Acc = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def JMPA : M86Instruction<
	0x00,
	(outs),
	(ins GPR:$Dst),
	"JMPA $Dst",
	[(brind GPR:$Dst)]
>;

//// JMPD - JuMP to Direct address, unconditional and inderect jump to address.
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1, Src = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def JMPD : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins m86_ctarget:$Acc),
	"JMPD $Dst $Acc",
	[]
>;

//// JMPI - JuMP to Indirect address, unconditional and inderect jump to address.
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1, Acc = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def JMPI : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins GPR:$Src),
	"JMPI $Dst $Src",
	[]
>;

//// RET - RETurn, unconditional jump to return address.
let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def RET : M86Microcode<
	(outs),
	(ins),
	"# RET",
	[(m86_ret)]
>, PseudoInstExpansion<(JMPA R1)>;

//// CALLD - CALL Directly the function, unconditional jump to function address with saving arguments.
let isCall = 1, Defs = [R0] in
def CALLD : M86Microcode<
	(outs),
	(ins m86_ctarget:$Acc),
	"# CALL",
	[(m86_call tglobaladdr:$Acc)]
>, PseudoInstExpansion<(JMPD R1, m86_ctarget:$Acc)>;

//// CALLI - CALL Inderectly the function, unconditional jump to function address with saving arguments.
let isCall = 1, Defs = [R0] in
def CALLI : M86Microcode<
	(outs),
	(ins GPR:$Dst),
	"# CALL (inderect)",
	[(m86_call GPR:$Dst)]
>, PseudoInstExpansion<(JMPI R1, GPR:$Dst)>;

//// JEQ - Jump if EQual, conditional jump if two operands equals.
def JEQ  : M86TemplateJCC<0x00, "JEQ">;

//// JGT - Jump if Greater Than, conditional jump if left operand greater than right operand.
def JGT  : M86TemplateJCC<0x00, "JGT">;

//// JGE - Jump if Greater/Equals, conditional jump if left operand greater than or equals right operand.
def JGE  : M86TemplateJCC<0x00, "JGE">;

//// JLT - Jump if Less Than, conditional jump if left operand less than right operand.
def JLT  : M86TemplateJCC<0x00, "JLT">;

//// JLE - Jump if Less/Equals, conditional jump if left operand less than or equals right operand.
def JLE  : M86TemplateJCC<0x00, "JLE">;

//// JNE - Jump if Not Equal, conditional jump if two operands not equals.
def JNE  : M86TemplateJCC<0x00, "JNE">;

//// JUEQ - Jump if Unsigned EQual, conditional jump if two operands equals.
def JUEQ  : M86TemplateJCC<0x00, "JUEQ">;

//// JUGT - Jump if Unsigned Greater Than, conditional jump if left operand greater than right operand.
def JUGT  : M86TemplateJCC<0x00, "JUGT">;

//// JUGE - Jump if Unsigned Greater/Equals, conditional jump if left operand greater than or equals right operand.
def JUGE  : M86TemplateJCC<0x00, "JUGE">;

//// JULT - Jump if Unsigned Less Than, conditional jump if left operand less than right operand.
def JULT  : M86TemplateJCC<0x00, "JULT">;

//// JULE - Jump if Unsigned Less/Equals, conditional jump if left operand less than or equals right operand.
def JULE  : M86TemplateJCC<0x00, "JULE">;

//// JUNE - Jump if Unsigned Not Equal, conditional jump if two operands not equals.
def JUNE  : M86TemplateJCC<0x00, "JUNE">;

def : M86PatternJCC<SETEQ,  JEQ>;
def : M86PatternJCC<SETGT,  JGT>;
def : M86PatternJCC<SETGE,  JGE>;
def : M86PatternJCC<SETLT,  JLT>;
def : M86PatternJCC<SETLE,  JLE>;
def : M86PatternJCC<SETNE,  JNE>;

def : M86PatternJCC<SETUEQ, JUEQ>;
def : M86PatternJCC<SETUGT, JUGT>;
def : M86PatternJCC<SETUGE, JUGE>;
def : M86PatternJCC<SETULT, JULT>;
def : M86PatternJCC<SETULE, JULE>;
def : M86PatternJCC<SETUNE, JUNE>;

// Memory.

//// MOVTLI - MOVe Low 16 bits Immediate value To register.
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, Src = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def MOVTLI : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins m86_i32li16:$Acc),
	"MOVTLI $Dst $Acc",
	[(set GPR:$Dst, m86_i32li16:$Acc)]
>;

//// MOVTHI - MOVe High 16 bits Immediate value To register.
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, Src = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def MOVTHI : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins m86_i32hi16:$Acc),
	"MOVTHI $Dst $Acc",
	[(set GPR:$Dst, m86_i32hi16:$Acc)]
>;

//// MEMST - MEMory STore value to.
let hasSideEffects = 0, mayLoad = 0, mayStore = 1, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def MEMST : M86Instruction<
	0x00,
	(outs),
	(ins GPR:$Dst, GPR:$Src, m86_simm:$Acc),
	"MEMST $Dst ${Src} ${Acc}",
	[]
>;

def : Pat<
	(store (i32 GPR:$Dst), m86_baseaddr:$Src),
	(MEMST GPR:$Dst, m86_baseaddr:$Src, 0)
>;

def : Pat<
	(store (i32 GPR:$Dst), (add m86_baseaddr:$Src, m86_simm:$Acc)),
	(MEMST GPR:$Dst, m86_baseaddr:$Src, m86_simm:$Acc)
>;

def : Pat<
	(store (i32 GPR:$Dst), (IsOrAdd m86_fiaddr:$Src, m86_simm:$Acc)),
	(MEMST GPR:$Dst, m86_fiaddr:$Src, m86_simm:$Acc)
>;

//// MEMLD - MEMory LoaD value from.
let hasSideEffects = 0, mayLoad = 1, mayStore = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def MEMLD : M86Instruction<
	0x00,
	(outs GPR:$Dst), (ins GPR:$Src, m86_simm:$Acc),
	"MEMLD $Dst ${Src} ${Acc}",
	[]
>;

def : Pat<
	(i32 (load GPR:$Src)),
	(MEMLD GPR:$Src, 0)
>;

def : Pat<
	(i32 (load (add m86_baseaddr:$Src, m86_simm:$Acc))),
	(MEMLD m86_baseaddr:$Src, m86_simm:$Acc)
>;

def : Pat<
	(i32 (load (IsOrAdd m86_fiaddr:$Src, m86_simm:$Acc))),
	(MEMLD m86_fiaddr:$Src, m86_simm:$Acc)
>;

// Arithmetics.

//// ADDI - register ADD with Immediate.
let isCommutable = 1, isReMaterializable = 0, Src = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def ADDI : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins GPR:$Src, m86_simm:$Acc),
	"ADD $Dst $Src $Acc",
	[
		(set GPR:$Dst, (add GPR:$Src, m86_simm:$Acc))
	]
>;

//// ADD - register ADD with register.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def ADD : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"ADD $Dst $Src",
	[
		(set GPR:$Dst_out, (add GPR:$Dst, GPR:$Src))
	]
>;

//// SUB - register SUB with register.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def SUB : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"SUB $Dst $Src",
	[
		(set GPR:$Dst_out, (sub GPR:$Dst, GPR:$Src))
	]
>;

//// MUL - MULtiply of two registers.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def MUL : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"MUL $Dst $Src",
	[
		(set GPR:$Dst_out, (mul GPR:$Dst, GPR:$Src))
	]
>;

//// XOR - bitXOR of two operands.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def XOR : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"XOR $Dst $Src",
	[
		(set GPR:$Dst_out, (xor GPR:$Dst, GPR:$Src))
	]
>;

//// OR - bitOR of two operands.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def OR : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"OR $Dst $Src",
	[
		(set GPR:$Dst_out, (or GPR:$Dst, GPR:$Src))
	]
>;

//// AND - bitAND of two operands.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def AND : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"XOR $Dst $Src",
	[
		(set GPR:$Dst_out, (and GPR:$Dst, GPR:$Src))
	]
>;

//// SHL - SHift Left.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def SHL : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"SHL $Dst $Src",
	[
		(set GPR:$Dst_out, (shl GPR:$Dst, GPR:$Src))
	]
>;

//// SHR - SHift Right.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def SHR : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"SHL $Dst $Src",
	[
		(set GPR:$Dst_out, (srl GPR:$Dst, GPR:$Src))
	]
>;

//// SRA - Shift Right Arithmetic.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def SRA : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"SRA $Dst $Src",
	[
		(set GPR:$Dst_out, (sra GPR:$Dst, GPR:$Src))
	]
>;

// Arithmetics. M86 extended.

//// EXOR - extended bitxor of two operands.
let isCommutable = 1, isReMaterializable = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def EXOR : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins GPR:$Src, GPR:$Acc),
	"EXOR $Dst $Src $Acc",
	[
		(set GPR:$Dst, (xor GPR:$Src, GPR:$Acc))
	]
>;

//// EMUL - extended multiply of two operands.
let isCommutable = 1, isReMaterializable = 0, AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def EMUL : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins GPR:$Src, GPR:$Acc),
	"EMUL $Dst $Src $Acc",
	[
		(set GPR:$Dst, (mul GPR:$Src, GPR:$Acc))
	]
>;

//// EADDMUL - extended add and multiply of two operands.
let isCommutable = 1, isReMaterializable = 0, Constraints = "$Dst = $Dst_out", AR1 = 0, AR2 = 0, AR3 = 0, AR4 = 0 in
def EADDMUL : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc),
	"EADDMUL $Dst $Src $Acc",
	[
		(set GPR:$Dst_out, (add GPR:$Dst, (mul GPR:$Src, GPR:$Acc)))
	]
>;

//===----------------------------------------------------------------------===//
// M86 Other patterns
//===----------------------------------------------------------------------===//

// i16 imm
def : Pat<(i32 m86_i32li16:$Acc), (MOVTLI imm:$Acc)>;

// i32 imm
def : Pat<(i32 imm:$Acc), (ADDI (MOVTHI (m86_immhi imm:$Acc)), (m86_immli imm:$Acc))>;

// Addresses
def : Pat<
	(add (i32 m86_fiaddr:$Src), (i32 m86_fiaddr:$Acc)),
	(ADDI (i32 m86_fiaddr:$Src), (i32 m86_fiaddr:$Acc))
>;

def : Pat<
	(add (i32 m86_fiaddr:$Src), m86_simm:$Acc),
	(ADDI (i32 m86_fiaddr:$Src), m86_simm:$Acc)
>;

def : Pat<
	(IsOrAdd (i32 m86_fiaddr:$Src), m86_simm:$Acc),
	(ADDI (i32 m86_fiaddr:$Src), m86_simm:$Acc)
>;

// Instructions.
def : Pat<
	(add GPR:$Dst, (i32 m86_simm:$Acc)),
	(ADD GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(add (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(ADD (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;

def : Pat<
	(sub GPR:$Dst, (i32 m86_simm:$Acc)),
	(SUB GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(sub (i32 m86_simm:$Acc), GPR:$Src),
	(SUB (MOVTLI imm:$Acc), GPR:$Src)
>;

def : Pat<
	(sub (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(SUB (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;

def : Pat<
	(xor  GPR:$Dst, (i32 m86_simm:$Acc)),
	(XOR GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(xor (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(XOR (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;

def : Pat<
	(or  GPR:$Dst, (i32 m86_simm:$Acc)),
	(OR GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(or (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(OR (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;

def : Pat<
	(and  GPR:$Dst, (i32 m86_simm:$Acc)),
	(AND GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(and (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(AND (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;

def : Pat<
	(mul  GPR:$Dst, (i32 m86_simm:$Acc)),
	(MUL GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(mul (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(MUL (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;

def : Pat<
	(shl GPR:$Dst, (i32 m86_simm:$Acc)),
	(SHL GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(shl (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(SHL (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;

def : Pat<
	(srl GPR:$Dst, (i32 m86_simm:$Acc)),
	(SHR GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(srl (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(SHR (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;

def : Pat<
	(sra GPR:$Dst, (i32 m86_simm:$Acc)),
	(SRA GPR:$Dst, (MOVTLI imm:$Acc))
>;

def : Pat<
	(sra (i32 m86_simm:$Src), (i32 m86_simm:$Acc)),
	(SRA (MOVTLI imm:$Src), (MOVTLI imm:$Acc))
>;
