/* modified by fwang12, Feifei Wang*/
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/ToolOutputFile.h"
#include <memory>
#include <algorithm>
using namespace llvm;

extern FILE *yyin;

extern int yyparse();
extern void initialize();

Module* M;

std::string OutputFilename;

int correct = 0;
int
main (int argc, char ** argv)
{
  if (argc < 3) {
    fprintf(stderr,"Not enough positional arguments to %s.\n",argv[0]);
    return 1;
  }
  
  // initialize relevant data structures in parser.y
  initialize();

  // Get input filename
  std::string InputFilename(argv[1]);

  // Get output filename.
  OutputFilename = argv[2];

  // LLVM idiom for constructing output file.
  OwningPtr<tool_output_file> Out;  
  std::string ErrorInfo;
  Out.reset(new tool_output_file(OutputFilename.c_str(), ErrorInfo,
  				 sys::fs::F_Binary));

  // Build new module
  M  = new Module("p1",getGlobalContext());
  
  // Set parser to InputFilename
  yyin = fopen(InputFilename.c_str(),"r");
  yyparse();

  // Dump function to bitcode
if(correct)
{
  WriteBitcodeToFile(M,Out->os()); 

  // LLVM idiom to save output file
  Out->keep();
}
  return 0;
}
