#ifndef PROFILER_H
#define PROFILER_H

#include "llvm/Support/DataTypes.h"
#include "llvm-c/Core.h"

#ifdef __cplusplus

/* Need these includes to support the LLVM 'cast' template for the C++ 'wrap' 
   and 'unwrap' conversion functions. */
#include "llvm/IR/Module.h"
#include "llvm/PassRegistry.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/ProfileInfo.h"

namespace llvm {

void ProfilerInit(ProfileInfo *PI);
double ProfilerGetBlockCount(BasicBlock* BB);
double ProfilerGetFunctionCount(Function *F);
bool HasProfileInfo();

}

extern "C" {
#endif

double LLVMProfilerGetBlockCount(LLVMBasicBlockRef BB);
double LLVMProfilerGetFunctionCount(LLVMValueRef Fun);
LLVMBool LLVMHasProfileInfo();

#ifdef __cplusplus
}
#endif

#endif
