#ifndef LLVM_LIB_TARGET_M86_M86TARGETSTREAMER_H
#define LLVM_LIB_TARGET_M86_M86TARGETSTREAMER_H

#include <llvm/MC/MCStreamer.h>

namespace llvm {

class M86TargetStreamer : public MCTargetStreamer {
public:
  M86TargetStreamer(MCStreamer &S);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_M86_M86TARGETSTREAMER_H
