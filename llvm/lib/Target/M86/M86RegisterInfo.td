// Abstract M86 register.
class M86Reg<bits<16> enc, string name_mnemonic, string name_abi = ""> : Register<name_mnemonic>
{
	let Namespace = "M86";
	let HWEncoding = enc;
	// let Name = name_abi;
}

// Special registers (read-only).
class M86SReg<bits<16> enc, string name_mnemonic, string name_abi> : M86Reg<enc, name_mnemonic, name_abi>;

// Program registers (read-user).
class M86PReg<bits<16> enc, string name_mnemonic, string name_abi> : M86Reg<enc, name_mnemonic, name_abi>;

// General purpose registers (read-write).
class M86GPRReg<bits<16> enc, string name_mnemonic, string name_abi> : M86Reg<enc, name_mnemonic, name_abi>;

// Constant zero register.
def R0 : M86SReg<0, "R0", "RZ">;

// Constant maximum value register.
def R1 : M86SReg<1, "R1", "RO">;

// Constant value register.
def R2 : M86PReg<2, "R2", "RI">;

// User registers.
def R3 : M86GPRReg<3, "R3", "R1">;
def R4 : M86GPRReg<4, "R4", "R2">;
def R5 : M86GPRReg<5, "R5", "R3">;
def R6 : M86GPRReg<6, "R6", "R4">;
def R7 : M86GPRReg<7, "R7", "R5">;
def R8 : M86GPRReg<8, "R8", "R6">;
def R9 : M86GPRReg<9, "R9", "R7">;
def R10 : M86GPRReg<10, "R10", "R8">;

// Stack pointer register.
def R11 : M86GPRReg<11, "R11", "RS">;

// Base stack pointer register.
def R12 : M86GPRReg<12, "R12", "RB">;

// Temporary flags register.
def R13 : M86PReg<13, "R13", "RF">;

// Not used (reserved for return-address).
def R14 : M86PReg<14, "R14", "RX">;
def R15 : M86PReg<15, "R15", "RY">;

// Special classes.
def SRegClass : RegisterClass<"M86", [i32], 32, (add R0, R1)>;

// Program classes.
def PRegClass : RegisterClass<"M86", [i32], 32, (add R2, R13, R14, R15)>;

// General purpose registers.
def GPRRegClass : RegisterClass<"M86", [i32], 32, (add R3, R4, R5, R6, R7, R8, R9, R10, R11, R12)>;
