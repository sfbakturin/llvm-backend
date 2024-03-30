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

	bits<8> Opcode  = op;
	bits<4> RegDst = 0;
	bits<4> RegSrc  = 0;
	bits<4> RegAcc  = 0;
	bits<24> Imm    = 0;

	let Inst{31-24} = Opcode;
	let Inst{23-20} = RegDst;
	let Inst{19-16} = RegSrc;
	let Inst{15-12} = RegAcc;
	let Inst{23-0}  = Imm;

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
