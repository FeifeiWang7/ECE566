#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "llvm-c/Core.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/Triple.h"

#include "llvm/IR/DataLayout.h"
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
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <memory>
#include <algorithm>
#include <memory>
#include <algorithm>
#include "llvm/Support/GraphWriter.h"
#include "llvm/Analysis/CFGPrinter.h"


using namespace llvm;

#include "summary.h"
#include "FT.h"

#define UseC

int
main (int argc, char ** argv)
{
  if (argc < 3) {
    fprintf(stderr,"Not enough positional arguments to %s.\n",argv[0]);
    return 1;
  }

  llvm_shutdown_obj Y;  // Call llvm_shutdown() on exit.
  LLVMContext &Context = getGlobalContext();

  std::string InputFilename(argv[1]);
  std::string OutputFilename(argv[2]);

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

  PassManager Passes;

  TargetLibraryInfo *TLI = new TargetLibraryInfo(Triple(M->getTargetTriple()));
  DataLayout *TD = 0;
  const std::string &ModuleDataLayout = M.get()->getDataLayout();
  TD = new DataLayout(ModuleDataLayout);

  if (TD)
    Passes.add(TD);

  //  Passes.add(createVerifierPass());
  PassManagerBuilder Builder;
  Builder.OptLevel=3;
  Builder.populateModulePassManager(Passes);
  //Builder.Inliner = createFunctionInliningPass();
  Builder.SizeLevel = 1;
  // Apply O3 optimizations
  Passes.run(*M.get());

  Module::iterator fit, fend;
  Function::iterator bit, bend;
  BasicBlock::iterator it, iend;

#ifdef UseC
  Swift_C(wrap(M.get())); // Uncomment this for C
#else
  Swift_Cpp(M.get()); // Use this for C++
#endif

  WriteBitcodeToFile(M.get(),Out->os());

  Out->keep();

  return 0;
}
