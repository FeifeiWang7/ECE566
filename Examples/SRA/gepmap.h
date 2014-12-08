#ifndef GEPMAP_H
#define GEPMAP_H

#include "llvm/Support/DataTypes.h"
#include "llvm-c/Core.h"

#ifdef __cplusplus

/* Need these includes to support the LLVM 'cast' template for the C++ 'wrap' 
   and 'unwrap' conversion functions. */
#include "llvm/IR/Module.h"
#include "llvm/PassRegistry.h"
#include "llvm/IR/IRBuilder.h"

extern "C" {
#endif

typedef void * GEPMap;

/* Create an empty worklist */
GEPMap GEPMapCreate();

void GEPMapDestroy(GEPMap);

/* Insert a new value into worklist */
int GEPMapInsert(GEPMap map, LLVMValueRef key, LLVMValueRef data);

/* Insert a new value into worklist */
LLVMValueRef GEPMapFind(GEPMap map, LLVMValueRef key);

#ifdef __cplusplus
}
#endif

#endif
