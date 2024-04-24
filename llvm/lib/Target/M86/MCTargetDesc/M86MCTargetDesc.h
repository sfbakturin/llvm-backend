#ifndef LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCTARGETDESC_H
#define LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCTARGETDESC_H

// Defines symbolic names for M86 registers.  This defines a mapping from
// register name to register number.
//

#define GET_REGINFO_ENUM
#include <M86GenRegisterInfo.inc>

// Defines symbolic names for the M86 instructions.
#define GET_INSTRINFO_ENUM
#include <M86GenInstrInfo.inc>

#include <cstdint>
#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;

MCCodeEmitter *createM86MCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx);
MCAsmBackend *createM86AsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                  const MCRegisterInfo &MRI,
                                  const MCTargetOptions &Options);
std::unique_ptr<MCObjectTargetWriter>
createM86ELFObjectWriter(bool Is64Bit, std::uint8_t OSABI);
} // namespace llvm

#endif // LLVM_LIB_TARGET_M86_MCTARGETDESC_M86MCTARGETDESC_H
