add_llvm_component_library(LLVMM86Desc
  M86MCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  Support
  M86Info

  ADD_TO_COMPONENT
  M86
 )
