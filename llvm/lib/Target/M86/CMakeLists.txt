add_llvm_component_group(M86)

add_llvm_target(M86CodeGen
  M86TargetMachine.cpp

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
