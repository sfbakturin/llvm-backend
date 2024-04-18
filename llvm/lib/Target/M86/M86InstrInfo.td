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

def m86_call          : SDNode<"M86ISD::CALL",       M86SDTCall,         [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue, SDNPVariadic]>;
def m86_callseq_end   : SDNode<"ISD::CALLSEQ_START", M86SDTCallSeqStart, [SDNPHasChain, SDNPOutGlue]>;
def m86_callseq_start : SDNode<"ISD::CALLSEQ_END",   M86SDTCallSeqEnd,   [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def m86_ret           : SDNode<"M86ISD::RET",        SDTNone,            [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def m86_jcc           : SDNode<"M86ISD::JCC",        M86SDTJCC,          [SDNPHasChain]>;

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

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBranch = 1, isTerminator = 1 in
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

// Control flow.

//// JMPL - JuMP to Label, unconditional jump by offset.
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBarrier = 1, isBranch = 1, isTerminator = 1, Dst = 0, Src = 0 in
def JMPL : M86Instruction<
	0x00,
	(outs),
	(ins m86_btarget:$Acc),
	"JMPL $Acc",
	[(br bb:$Acc)]
>;

//// JMPA - JuMP to Address, unconditional jump to address.
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1, Src = 0, Acc = 0 in
def JMPA : M86Instruction<
	0x00,
	(outs),
	(ins GPR:$Dst),
	"JMPA $Dst",
	[(brind GPR:$Dst)]
>;

//// RET - RETurn, unconditional jump to return address.
let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def RET : M86Microcode<
	(outs),
	(ins),
	"# PseudoRET",
	[(m86_ret)]
>, PseudoInstExpansion<(JMPA R1)>;

//// JEQ - Jump if EQual, conditional jump if two operands equals.
def JEQ  : M86TemplateJCC<0x00, "JEQ">;
//// JNE - Jump if Not Equal, conditional jump if two operands not equals.
def JNE  : M86TemplateJCC<0x00, "JNE">;
//// JLE - Jump if LEss, conditional jump if left operand less than right operand.
def JLE  : M86TemplateJCC<0x00, "JLE">;
//// JGT - Jump if GreaTer, conditional jump if left operand greater than right operand.
def JGT  : M86TemplateJCC<0x00, "JGT">;
def JLEU : M86TemplateJCC<0x00, "JLEU">;
def JGTU : M86TemplateJCC<0x00, "JGTU">;

def : M86PatternJCC<SETEQ,  JEQ>;
def : M86PatternJCC<SETNE,  JNE>;
def : M86PatternJCC<SETLE,  JLE>;
def : M86PatternJCC<SETGT,  JGT>;
def : M86PatternJCC<SETULE, JLEU>;
def : M86PatternJCC<SETUGT, JGTU>;

// Memory.

//// MOVTLI - MOVe Low 16 bits Immediate value To register.
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, Src = 0 in
def MOVTLI : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins m86_i32li16:$Acc),
	"MOVTLI $Dst $Acc",
	[(set GPR:$Dst, m86_i32li16:$Acc)]
>;

//// MOVLI - MOVe Low 16 bits Immediate value to R2 register.
//// def MOVLI : M86Microcode<
//// 	(outs),
//// 	(ins m86_i32li16:$Acc),
//// 	"# Microcode MOVLI",
//// 	[]
//// >, PseudoInstExpansion<(MOVTLI R2, $Acc)>;

//// MOVTHI - MOVe High 16 bits Immediate value To register.
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, Src = 0  in
def MOVTHI : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins m86_i32hi16:$Acc),
	"MOVTHI $Dst $Acc",
	[(set GPR:$Dst, m86_i32hi16:$Acc)]
>;

//// MOVHI - MOVe High 16 bits Immediate value to R2 register.
//// def MOVHI : M86Microcode<
//// 	(outs),
//// 	(ins m86_i32hi16:$Acc),
//// 	"# Microcode MOVHI",
//// 	[]
//// >, PseudoInstExpansion<(MOVTHI R2, $Acc)>;

//// MOVTF - MOVe value From register To register.
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, Acc = 0 in
def MOVTF : M86Instruction<
	0x00,
	(outs GPR:$Dst),
	(ins GPR:$Src),
	"MOVTF $Dst $Src",
	[(set GPR:$Dst, GPR:$Src)]
>;

//// MEMST - MEMory STore value to.
let hasSideEffects = 0, mayLoad = 0, mayStore = 1 in
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
let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in
def MEMLD : M86Instruction<
	0x00,
	(outs GPR:$Dst), (ins GPR:$Src, m86_simm:$Acc),
	"MEMLD $Dst ${Src} ${Acc}",
	[]
>;
def : Pat<
	(i32 (load m86_baseaddr:$Src)),
	(MEMLD m86_baseaddr:$Src, 0)
>;
def : Pat<
	(i32 (load (add m86_baseaddr:$Src, m86_simm:$Acc))),
	(MEMLD m86_baseaddr:$Src, m86_simm:$Acc)>;
def : Pat<
	(i32 (load (IsOrAdd m86_fiaddr:$Src, m86_simm:$Acc))),
	(MEMLD m86_fiaddr:$Src, m86_simm:$Acc)
>;

// Arithmetics.

//// ADDR - register ADD with Register.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out" in
def ADDR : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"ADD $Dst $Src",
	[
		(set GPR:$Dst_out, (add GPR:$Dst, GPR:$Src))
	]
>;

//// ADDI - register ADD with Immediate.
let isCommutable = 1, isReMaterializable = 0, Src = 0, Constraints = "$Dst = $Dst_out" in
def ADDI : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, m86_simm:$Acc),
	"ADD $Dst $Acc",
	[
		(set GPR:$Dst_out, (add GPR:$Dst, m86_simm:$Acc))
	]
>;

//// MUL - multiply of two operands.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out" in
def MUL : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"MUL $Dst $Src",
	[
		(set GPR:$Dst_out, (mul GPR:$Dst, GPR:$Src))
	]
>;

//// XOR - bitxor of two operands.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out" in
def XOR : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"XOR $Dst $Src",
	[
		(set GPR:$Dst_out, (xor GPR:$Dst, GPR:$Src))
	]
>;

//// OR - bitor of two operands.
let isCommutable = 1, isReMaterializable = 0, Acc = 0, Constraints = "$Dst = $Dst_out" in
def OR : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src),
	"OR $Dst $Src",
	[
		(set GPR:$Dst_out, (or GPR:$Dst, GPR:$Src))
	]
>;

// Arithmetics. M86 extended.

//// EXOR - extended bitxor of two operands.
let isCommutable = 1, isReMaterializable = 0 in
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
let isCommutable = 1, isReMaterializable = 0 in
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
let isCommutable = 1, isReMaterializable = 0, Constraints = "$Dst = $Dst_out" in
def EADDMUL : M86Instruction<
	0x00,
	(outs GPR:$Dst_out),
	(ins GPR:$Dst, GPR:$Src, GPR:$Acc),
	"EADDMUL $Dst $Src $Acc",
	[
		(set GPR:$Dst_out, (add GPR:$Dst, (mul GPR:$Src, GPR:$Acc)))
	]
>;

