add_llvm_component_library(LLVMM86Desc
  M86MCTargetDesc.cpp
  M86ELFMCAsmInfo.cpp
  M86InstPrinter.cpp
  M86MCCodeEmitter.cpp
  M86AsmBackend.cpp
  M86ELFObjectWriter.cpp
  M86ELFAsmBackend.cpp
  M86MCTargetDesc.h
  M86ELFMCAsmInfo.h
  M86InstPrinter.h
  M86MCCodeEmitter.h
  M86AsmBackend.h
  M86ELFObjectWriter.h
  M86ELFAsmBackend.h
  M86FixupKinds.h

  LINK_COMPONENTS
  MC
  Support
  M86Info

  ADD_TO_COMPONENT
  M86
 )
