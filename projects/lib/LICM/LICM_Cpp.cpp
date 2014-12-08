/*
 * File: LICM_Cpp.cpp
 *
 * Description:
 *   Stub for LICM in Cpp. Extend to provide LICM implementation.
 */

#include <stdio.h>
#include <stdlib.h>

/* LLVM Header Files */
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Type.h"

using namespace llvm;

unsigned int LICM_Count=0;
unsigned int LICM_NoPreheader=0;
unsigned int LICM_AfterLoop=0;
unsigned int LICM_Load=0;
unsigned int LICM_BadCall=0;
unsigned int LICM_BadStore=0;

void LoopInvariantCodeMotion_Cpp(Module* M)
{

  fprintf(stderr,"LICM_Count      =%d\n",LICM_Count);
  fprintf(stderr,"LICM_NoPreheader=%d\n",LICM_NoPreheader);
  fprintf(stderr,"LICM_Load       =%d\n",LICM_Load);
  fprintf(stderr,"LICM_BadCall    =%d\n",LICM_BadCall);
  fprintf(stderr,"LICM_BadStore   =%d\n",LICM_BadStore);
}
