#ifndef GCM_H
#define GCM_H

#include "llvm/Support/DataTypes.h"
#include "llvm-c/Core.h"

#ifdef __cplusplus

/* Need these includes to support the LLVM 'cast' template for the C++ 'wrap' 
   and 'unwrap' conversion functions. */
#include "llvm/IR/Module.h"
#include "llvm/PassRegistry.h"
#include "llvm/IR/IRBuilder.h"

void GlobalCodeMotion_Cpp(Module*);

extern "C" {
#endif

void GlobalCodeMotion_C(LLVMModuleRef Module);

#ifdef __cplusplus
}
#endif

#endif
