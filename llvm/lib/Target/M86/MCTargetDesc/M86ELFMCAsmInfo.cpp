#include <M86ELFMCAsmInfo.h>

llvm::M86ELFMCAsmInfo::M86ELFMCAsmInfo(const llvm::Triple &TT) {
  SupportsDebugInformation = false;
  Data16bitsDirective = "\t.word\t";
  Data32bitsDirective = "\t.dword\t";
  Data64bitsDirective = "\t.qword\t";
  ZeroDirective = "\t.zero\t";
  CommentString = ";";

  UsesELFSectionDirectiveForBSS = false;
  AllowAtInName = true;
  HiddenVisibilityAttr = llvm::MCSA_Invalid;
  HiddenDeclarationVisibilityAttr = llvm::MCSA_Invalid;
  ProtectedVisibilityAttr = llvm::MCSA_Invalid;

  ExceptionsType = llvm::ExceptionHandling::None;
}
