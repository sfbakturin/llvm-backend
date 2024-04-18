// Format: OPCODE    <-- 1 bytes
//         ARGUMENTS <-- 3 bytes
// Two types of instructions: IMM (opcode+imm), LBL (opcode+label), REG (opcode+reg1+reg2+reg3).

// IMM: FF FF FF FF
//         ^^ ^^ ^^ - 24-bit number
//      ^^          - 8-bit opcode

// REG: FF FF Fx xx
//             ^ ^^ - 12-bit not used
//            ^     - 4-bit accumulator register
//          ^       - 4-bit source register
//         ^        - 4-bit destination register
//      ^^          - 8-bit opcode

// LBL: FF FF FF FF
//         ^^ ^^ ^^ - 24-bit offset address
//      ^^          - 8-bit opcode

// General M86 instruction format.
class M86Instruction<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern> : Instruction
{
	let Namespace        = "M86";
	let DecoderNamespace = "M86";
	let Size             = 4;

	field bits<32> Inst;

	bits<8>  Opcode = op;
	bits<4>  Dst;
	bits<4>  Src;
	bits<16> Acc;

	let Inst{31-24} = Opcode;
	let Inst{23-20} = Dst;
	let Inst{19-16} = Src;
	let Inst{15-0}  = Acc;

	let OutOperandList = outs;
	let InOperandList  = ins;
	let AsmString      = asmstr;
	let Pattern        = pattern;
}

// Microcode M86 instruction format.
class M86Microcode<dag outs, dag ins, string asmstr, list<dag> pattern> : M86Instruction<0, outs, ins, asmstr, pattern>
{
	let isPseudo = 1;
	let isCodeGenOnly = 1;
}
