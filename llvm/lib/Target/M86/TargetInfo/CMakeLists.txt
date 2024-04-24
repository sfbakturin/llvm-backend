add_llvm_component_library(LLVMM86Info
  M86TargetInfo.cpp
  M86TargetInfo.h

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  M86
  )
