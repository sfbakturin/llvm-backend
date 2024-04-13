add_llvm_component_group(M86)

set(LLVM_TARGET_DEFINITIONS M86.td)

option(M86_DEBUG_START_FUNCTION "Prints function name of M86 arch that is called with filename and lineno" OFF)

tablegen(LLVM M86GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM M86GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM M86GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM M86GenCallingConv.inc -gen-callingconv)
tablegen(LLVM M86GenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM M86GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM M86GenDAGISel.inc -gen-dag-isel)

add_public_tablegen_target(M86CommonTableGen)

add_llvm_target(M86CodeGen
  M86TargetMachine.cpp
  M86RegisterInfo.cpp
  M86FrameLowering.cpp
  M86InstrInfo.cpp
  M86Subtarget.cpp
  M86AsmPrinter.cpp
  M86FrameLowering.cpp
  M86InstrLowering.cpp
  M86MachineFunctionInfo.cpp
  M86TargetStreamer.cpp
  M86ISelDagToDag.cpp
  M86ISelLowering.cpp
  M86PassConfig.cpp

  LINK_COMPONENTS
  AsmPrinter
  CodeGen
  Core
  MC
  M86Desc
  SelectionDAG
  Support
  Target
  M86Info

  ADD_TO_COMPONENT
  M86
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
