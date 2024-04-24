let TargetPrefix = "m86" in {  // All intrinsics start with "llvm.m86.".

def int_m86_putpixel: ClangBuiltin<"putpixel">, Intrinsic<[],            [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;
def int_m86_flush:    ClangBuiltin<"flush">,    Intrinsic<[],            [],                                      [IntrNoMem, IntrHasSideEffects]>;
def int_m86_simrand:  ClangBuiltin<"simrand">,  Intrinsic<[llvm_i32_ty], [],                                      [IntrNoMem, IntrHasSideEffects]>;
def int_m86_select:   ClangBuiltin<"select">,   Intrinsic<[llvm_i32_ty], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

} // TargetPrefix = "m86"
