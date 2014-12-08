#include <stdio.h>
#include <stdlib.h>

/* LLVM Header Files */
#include "llvm-c/Core.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Type.h"

using namespace llvm;

extern "C" {
  LLVMValueRef LLVMGetCalledFunction(LLVMValueRef I);
}

LLVMValueRef LLVMGetCalledFunction(LLVMValueRef I)
{
  CallInst *CI = (CallInst*)unwrap(I);
  return wrap(CI->getCalledValue());
}
