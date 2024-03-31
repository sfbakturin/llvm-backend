include "llvm/Target/Target.td"
include "M86RegisterInfo.td"
include "M86InstrInfo.td"

class M86Processor<string Name, list<SubtargetFeature> Features> : Processor<Name, NoItineraries, Features>;

def : M86Processor<"generic", []>;

def M86InstrInfo : InstrInfo;

def M86 : Target
{
	let InstructionSet = M86InstrInfo;
}
