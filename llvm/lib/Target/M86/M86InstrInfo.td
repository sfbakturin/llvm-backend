// M86 instruction formats.
include "M86InstrFormats.td"

// M86 profiles and nodes.
def M86RetNode : SDNode<"M86ISD:RET", SDTNone, [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

// M86 operands, complex patterns, transformations definitions.
class M86ImmAsmOperand<string name> : AsmOperandClass
{
	let Name = name;
	let RenderMethod = "addImmOperands";
	let DiagnosticType = !strconcat("Invalid", Name);
}

def M86Imm : Operand<i32>, ImmLeaf<i32, [{ return isInt<24>(Imm); }]>
{
	let ParserMatchClass = M86ImmAsmOperand<"M86Imm">;
	let EncoderMethod = "getImmOpValue";
	let DecoderMethod = "decodeM86ImmOperand<24>";
	let MCOperandPredicate =
	[{
			int64_t Imm;
			if (MCOp.evaluateAsConstantImm(Imm))
			{
				return isInt<24>(Imm);
			}
			return MCOp.isBareSymbolRef();
	}];
	let OperandType = "OPERAND_M86IMM";
	let OperandNamespace = "M86Op";
}

// M86 instructions.

//// Memory control.

////// MOV imm        ---> store IMM value to R2 register.
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1 in
def MOV : M86Instruction<
	0x0,
	(outs GPRRegClass:$src),
	(ins M86Imm:$imm),
	"MOV $imm",
	[
		(set GPRRegClass:$src, M86Imm:$imm)
	]
>;

////// LOADcc src dst  ---> load cc from MEMORY dst to src register.
////// TODO

////// STOREcc src dst ---> store cc from dst register to MEMORY src
////// TODO

////// CMP src dst     ---> compare src register and dst register and set RF register (flags).
////// TODO

//// Control flow.

////// JMP src         ---> unconditional jump by offset address.
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1 in
def JMP : M86Instruction<
	0x0, // opcode
	(outs), // output
	(ins GPRRegClass:$src), // input
	"JMP $src", // asm string view
	[
		(brind GPRRegClass:$src)
	] // dag pattern
>;

////// Jcc src         ---> conditional cc jump by offset address, if cc is TRUE.
////// TODO

////// RET             ---> return.
let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def MicrocodeRET : M86Microcode<(outs), (ins), "# MicrocodeRET", [(M86RetNode)]>, PseudoInstExpansion<(JMP R14, R0, R0)>;

//// Arithmetics. Basics.

////// IADD src dst     ---> sum of two operands (as signed values), src += dst.
let isCommutable = 1, isReMaterializable = 0 in
def IADD : M86Instruction<
	0x0,
	(outs GPRRegClass:$src_out),
	(ins GPRRegClass:$src, GPRRegClass:$dst),
	"IADD $src $dst",
	[
		(set GPRRegClass:$src_out, (add GPRRegClass:$src, GPRRegClass:$dst))
	]
>; // TODO: make it working as signed value with changing flags

////// UADD src dst     ---> sum of two operands (as unsigned values), src += dst.
let isCommutable = 1, isReMaterializable = 0 in
def UADD : M86Instruction<
	0x0,
	(outs GPRRegClass:$src_out),
	(ins GPRRegClass:$src, GPRRegClass:$dst),
	"UADD $src $dst",
	[
		(set GPRRegClass:$src_out, (add GPRRegClass:$src, GPRRegClass:$dst))
	]
>; // TODO: make it working as unsigned value with changing flags

////// IMUL src dst     ---> multiply of two operands (as signed values), src *= dst.
let isCommutable = 1, isReMaterializable = 0 in
def IMUL : M86Instruction<
	0x0,
	(outs GPRRegClass:$src_out),
	(ins GPRRegClass:$src, GPRRegClass:$dst),
	"IMUL $src $dst",
	[
		(set GPRRegClass:$src_out, (mul GPRRegClass:$src, GPRRegClass:$dst))
	]
>; // TODO: make it working as signed value with changing flags

////// UMUL src dst     ---> multiply of two operands (as unsigned values), src *= dst.
let isCommutable = 1, isReMaterializable = 0 in
def UMUL : M86Instruction<
	0x0,
	(outs GPRRegClass:$src_out),
	(ins GPRRegClass:$src, GPRRegClass:$dst),
	"UMUL $src $dst",
	[
		(set GPRRegClass:$src_out, (mul GPRRegClass:$src, GPRRegClass:$dst))
	]
>; // TODO: make it working as unsigned value with changing flags

////// XOR src dst      ---> bitxor of two operands, src ^= dst.
let isCommutable = 1, isReMaterializable = 0 in
def XOR : M86Instruction<
	0x0,
	(outs GPRRegClass:$src_out),
	(ins GPRRegClass:$src, GPRRegClass:$dst),
	"XOR $src $dst",
	[
		(set GPRRegClass:$src_out, (xor GPRRegClass:$src, GPRRegClass:$dst))
	]
>; // TODO: make it working as unsigned value with changing flags

////// OR src dst       ---> bitand of two operands, src &= dst.
let isCommutable = 1, isReMaterializable = 0 in
def OR : M86Instruction<
	0x0,
	(outs GPRRegClass:$src_out),
	(ins GPRRegClass:$src, GPRRegClass:$dst),
	"OR $src $dst",
	[
		(set GPRRegClass:$src_out, (or GPRRegClass:$src, GPRRegClass:$dst))
	]
>; // TODO: make it working as unsigned value with changing flags

//// Arithmetics. M86 extended.

////// EXOR src dst acc  ---> extended bitxor of two operands, src = dst ^ acc.
let isCommutable = 1, isReMaterializable = 0 in
def EXOR : M86Instruction<
	0x0,
	(outs GPRRegClass:$src),
	(ins GPRRegClass:$dst, GPRRegClass:$acc),
	"EXOR $src $dst $acc",
	[
		(set GPRRegClass:$src, (xor GPRRegClass:$dst, GPRRegClass:$acc))
	]
>; // TODO: make it working as unsigned value with changing flags

////// EUMUL src dst acc ---> extended multiply of two operands (as unsigned values), src = dst * acc.
let isCommutable = 1, isReMaterializable = 0 in
def EUMUL : M86Instruction<
	0x0,
	(outs GPRRegClass:$src),
	(ins GPRRegClass:$dst, GPRRegClass:$acc),
	"EUMUL $src $dst $acc",
	[
		(set GPRRegClass:$src, (mul GPRRegClass:$dst, GPRRegClass:$acc))
	]
>; // TODO: make it working as unsigned value with changing flags

////// EUADDUMUL src dst acc ---> extended add and multiply of two operands (as unsigned values), src += dst * acc.
let isCommutable = 1, isReMaterializable = 0 in
def EUADDUMUL : M86Instruction<
	0x0,
	(outs GPRRegClass:$src_out),
	(ins GPRRegClass:$src, GPRRegClass:$dst, GPRRegClass:$acc),
	"EUADDUMUL $src $dst $acc",
	[
		(set GPRRegClass:$src_out, (add GPRRegClass:$src, (mul GPRRegClass:$dst, GPRRegClass:$acc)))
	]
>; // TODO: make it working as unsigned value with changing flags

////// INCZ src dst          ---> check if dst == 0 and increment src if TRUE, src += dst == 0.
////// TODO
