let TargetPrefix = "m86" in {  // All intrinsics start with "llvm.sim.".

def int_m86_putpixel: ClangBuiltin<"put_pixel">,
  Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

def int_m86_flush: ClangBuiltin<"flush">,
  Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

def int_m86_sim_rand: ClangBuiltin<"sim_rand">,
  Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

} // TargetPrefix = "m86"
