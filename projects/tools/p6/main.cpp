#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "llvm-c/Core.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/DebugInfo.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/RegionPass.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Target/TargetLibraryInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/PassNameParser.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/Debug.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/LinkAllPasses.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <memory>
#include <algorithm>
using namespace llvm;

#define UseC

#include "summary.h"
#include "CSE.h"
#include "LICM.h"
#include "GCM.h"

static cl::opt<std::string>
InputFilename(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));

static cl::opt<std::string>
OutputFilename(cl::Positional, cl::desc("<output bitcode>"), cl::init("out.bc"));

static cl::opt<bool>
NoSLP("no-slp",
  cl::desc("Do not perform superword level parallelization."),
  cl::init(false));

static cl::opt<bool>
DumpSummary("summary",
  cl::desc("Dump summary stats."),
  cl::init(false));


extern "C" void SLP_C(LLVMModuleRef Module);
extern void SLP_Cpp(Module *M);

int
main (int argc, char ** argv)
{
  cl::ParseCommandLineOptions(argc, argv, "llvm system compiler\n");

  llvm_shutdown_obj Y;  // Call llvm_shutdown() on exit.
  LLVMContext &Context = getGlobalContext();

  OwningPtr<tool_output_file> Out;  
  std::string ErrorInfo;
  Out.reset(new tool_output_file(OutputFilename.c_str(), ErrorInfo,
				 sys::fs::F_Binary));

  SMDiagnostic Err;

  std::auto_ptr<Module> M;
  M.reset(ParseIRFile(InputFilename, Err, Context));

  if (M.get() == 0) {
    Err.print(argv[0], errs());
    return 1;
  }

  if(!NoSLP) {
#ifdef UseC
    SLP_C(wrap(M.get()));
#else
    SLP_Cpp(wrap(M.get()));
#endif 
  } 

 if (DumpSummary)
   {
      char filename[1024]; 
      sprintf(filename,"%s.stats",OutputFilename.c_str());
#ifdef UseC
      Summarize(wrap(M.get()),"postSLP",filename);
#else
      Summarize_Cpp(M.get(),"postSLP",filename);
#endif
   }
   
   WriteBitcodeToFile(M.get(),Out->os());
   Out->keep();
   
   return 0;
}
