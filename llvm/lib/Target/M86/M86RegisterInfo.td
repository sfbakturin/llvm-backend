// Abstract M86 register.
class M86Reg<bits<16> enc, string name_mnemonic> : Register<name_mnemonic>
{
	let Namespace = "M86";
	let HWEncoding = enc;
}

// Registers.
def R0 : M86Reg<0, "R0">, DwarfRegNum<[0]>;
def R1 : M86Reg<1, "R1">, DwarfRegNum<[1]>;
def R2 : M86Reg<2, "R2">, DwarfRegNum<[2]>;
def R3 : M86Reg<3, "R3">, DwarfRegNum<[3]>;
def R4 : M86Reg<4, "R4">, DwarfRegNum<[4]>;
def R5 : M86Reg<5, "R5">, DwarfRegNum<[5]>;
def R6 : M86Reg<6, "R6">, DwarfRegNum<[6]>;
def R7 : M86Reg<7, "R7">, DwarfRegNum<[7]>;
def R8 : M86Reg<8, "R8">, DwarfRegNum<[8]>;
def R9 : M86Reg<9, "R9">, DwarfRegNum<[9]>;
def R10 : M86Reg<10, "R10">, DwarfRegNum<[10]>;
def R11 : M86Reg<11, "R11">, DwarfRegNum<[11]>;
def R12 : M86Reg<12, "R12">, DwarfRegNum<[12]>;
def R13 : M86Reg<13, "R13">, DwarfRegNum<[13]>;
def R14 : M86Reg<14, "R14">, DwarfRegNum<[14]>;
def R15 : M86Reg<15, "R15">, DwarfRegNum<[15]>;

// General purpose registers.
def GPR : RegisterClass<"M86", [i32], 32, (add R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15)>;
