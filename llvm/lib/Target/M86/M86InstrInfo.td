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

class M86ImmOperand : Operand<i32>, ImmLeaf<i32, [{ return isInt<24>(Imm); }]>
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
	let OperandType = "M86IMM";
	let OperandNamespace = "M86Op";
}

// M86 instructions.

//// JMP ---> control flow
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1 in
def JMP : M86Instruction<
	0x0, // opcode
	(outs), // output
	(ins GPRRegClass:$src), // input
	"JMP $src $dst $acc", // asm string view
	[
		(brind GPRRegClass:$src)
	] // dag pattern
>;

//// RET ---> return point
let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def MicrocodeRET : M86Microcode<(outs), (ins), "# MicrocodeRET", [(M86RetNode)]>, PseudoInstExpansion<(JMP R14, R0, R0)>;
