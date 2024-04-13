add_llvm_component_library(LLVMM86Desc
  M86MCTargetDesc.cpp
  M86ELFMCAsmInfo.cpp
  M86InstPrinter.cpp

  LINK_COMPONENTS
  MC
  Support
  M86Info

  ADD_TO_COMPONENT
  M86
 )
