#ifndef FT_H
#define FT_H

#include "llvm/Support/DataTypes.h"
#include "llvm-c/Core.h"

#ifdef __cplusplus

#include "llvm/IR/Module.h"
#include "llvm/PassRegistry.h"
#include "llvm/IR/IRBuilder.h"

void Swift_Cpp(Module*);

extern "C" {
#endif

void Swift_C(LLVMModuleRef M);

#ifdef __cplusplus
}
#endif

#endif
