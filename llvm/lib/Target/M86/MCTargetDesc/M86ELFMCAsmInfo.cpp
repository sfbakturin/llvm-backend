#include <M86.h>
#include <MCTargetDesc/M86ELFMCAsmInfo.h>
#include <llvm/MC/MCDirectives.h>
#include <llvm/MC/MCTargetOptions.h>
#include <llvm/TargetParser/Triple.h>

using namespace llvm;

llvm::M86ELFMCAsmInfo::M86ELFMCAsmInfo(const llvm::Triple &TT) {
  M86_START_FUNCTION();

  SupportsDebugInformation = false;
  Data16bitsDirective = "\t.short\t";
  Data32bitsDirective = "\t.word\t";
  Data64bitsDirective = nullptr;
  ZeroDirective = "\t.space\t";
  CommentString = ";";

  UsesELFSectionDirectiveForBSS = false;
  AllowAtInName = true;
  HiddenVisibilityAttr = llvm::MCSA_Invalid;
  HiddenDeclarationVisibilityAttr = llvm::MCSA_Invalid;
  ProtectedVisibilityAttr = llvm::MCSA_Invalid;

  ExceptionsType = llvm::ExceptionHandling::None;

  M86_END_FUNCTION();
}
