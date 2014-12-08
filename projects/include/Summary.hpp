#ifndef SUMMARY_HPP
#define SUMMARY_HPP

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"

void Summarize_Cpp(llvm::Module *M, const char *id, const char *filename);

#endif // SUMMARY_H
