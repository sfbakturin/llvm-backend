#include <M86Subtarget.h>
#include <llvm/Target/TargetMachine.h>

#define DEBUG_TYPE "m86-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include <M86GenSubtargetInfo.inc>
#include <llvm/ADT/StringRef.h>

llvm::M86Subtarget::M86Subtarget(const llvm::StringRef &CPU,
                                 const llvm::StringRef &TuneCPU,
                                 const llvm::StringRef &FS,
                                 const llvm::TargetMachine &TM)
    : llvm::M86GenSubtargetInfo(TM.getTargetTriple(), CPU, TuneCPU, FS) {}
