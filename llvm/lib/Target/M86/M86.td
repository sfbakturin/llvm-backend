include "llvm/Target/Target.td"
include "M86RegisterInfo.td"
include "M86InstrInfo.td"

def M86InstrInfo : InstrInfo;

def M86 : Target
{
	let InstructionSet = M86InstrInfo;
}
