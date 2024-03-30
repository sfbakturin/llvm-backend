add_llvm_component_group(M86)

set(LLVM_TARGET_DEFINITIONS M86.td)

tablegen(LLVM M86GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM M86GenInstrInfo.inc -gen-instr-info)

add_public_tablegen_target(M86CommonTableGen)

add_llvm_target(M86CodeGen
  M86TargetMachine.cpp
  M86RegisterInfo.cpp
  M86InstrInfo.cpp

  LINK_COMPONENTS
  CodeGen
  Core
  Support
  Target
  M86Info

  ADD_TO_COMPONENT
  M86
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
