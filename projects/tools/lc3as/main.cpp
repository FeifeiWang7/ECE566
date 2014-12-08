#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "llvm-c/Core.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/CodeGen/CommandFlags.h"
#include "llvm/CodeGen/LinkAllAsmWriterComponents.h"
#include "llvm/CodeGen/LinkAllCodegenComponents.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/MC/SubtargetFeature.h"
#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Target/TargetLibraryInfo.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>


/*#include "llvm/IR/LLVMContext.h"
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
#include "llvm/Support/FormattedStream.h"
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
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Target/TargetLibraryInfo.h"
#include "llvm/Target/TargetMachine.h"

#include "llvm/LinkAllPasses.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"*/
#include <memory>
#include <algorithm>
using namespace llvm;

#define UseC

#include "summary.h"
#include "CSE.h"
#include "GCM.h"

static cl::opt<std::string>
InputFilename(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));

static cl::opt<std::string>
OutputFilename(cl::Positional, cl::desc("<output bitcode>"), cl::init("out.bc"));

// GetFileNameRoot - Helper function to get the basename of a filename.
static inline std::string
GetFileNameRoot(const std::string &InputFilename) {
  std::string IFN = InputFilename;
  std::string outputFilename;
  int Len = IFN.length();
  if ((Len > 2) &&
      IFN[Len-3] == '.' &&
      ((IFN[Len-2] == 'b' && IFN[Len-1] == 'c') ||
       (IFN[Len-2] == 'l' && IFN[Len-1] == 'l'))) {
    outputFilename = std::string(IFN.begin(), IFN.end()-3); // s/.bc/.s/
  } else {
    outputFilename = IFN;
  }
  return outputFilename;
}

static tool_output_file *GetOutputStream(const char *TargetName,
                                         Triple::OSType OS,
                                         const char *ProgName) {
  // If we don't yet have an output filename, make one.
  if (OutputFilename.empty()) {
    if (InputFilename == "-")
      OutputFilename = "-";
    else {
      OutputFilename = GetFileNameRoot(InputFilename);

      switch (FileType) {
      case TargetMachine::CGFT_AssemblyFile:
        if (TargetName[0] == 'c') {
          if (TargetName[1] == 0)
            OutputFilename += ".cbe.c";
          else if (TargetName[1] == 'p' && TargetName[2] == 'p')
            OutputFilename += ".cpp";
          else
            OutputFilename += ".s";
        } else
          OutputFilename += ".s";
        break;
      case TargetMachine::CGFT_ObjectFile:
        if (OS == Triple::Win32)
          OutputFilename += ".obj";
        else
          OutputFilename += ".o";
        break;
      case TargetMachine::CGFT_Null:
        OutputFilename += ".null";
        break;
      }
    }
  }

  // Decide if we need "binary" output.
  bool Binary = false;
  switch (FileType) {
  case TargetMachine::CGFT_AssemblyFile:
    break;
  case TargetMachine::CGFT_ObjectFile:
  case TargetMachine::CGFT_Null:
    Binary = true;
    break;
  }

  // Open the file.
  std::string error;
  sys::fs::OpenFlags OpenFlags = sys::fs::F_None;
  if (Binary)
    OpenFlags |= sys::fs::F_Binary;
  tool_output_file *FDOut = new tool_output_file(OutputFilename.c_str(), error,
                                                 OpenFlags);
  if (!error.empty()) {
    errs() << error << '\n';
    delete FDOut;
    return 0;
  }

  return FDOut;
}


extern "C" void LLVMInitializeLC3TargetInfo();
extern "C" void LLVMInitializeLC3Target();
extern "C" void LLVMInitializeLC3TargetMC();
extern "C" void LLVMInitializeLC3AsmPrinter();

int
main (int argc, char ** argv)
{
  sys::PrintStackTraceOnErrorSignal();
  PrettyStackTraceProgram X(argc, argv);

  // Enable debug stream buffering.
  EnableDebugBuffering = true;

  LLVMContext &Context = getGlobalContext();
  llvm_shutdown_obj Y;  // Call llvm_shutdown() on exit.

  LLVMInitializeLC3Target();
  LLVMInitializeLC3TargetInfo();
  LLVMInitializeLC3TargetMC();
  LLVMInitializeLC3AsmPrinter();
  //LLVMInitializeLC3TargetAsmParser();

    // Initialize codegen and IR passes used by llc so that the -print-after,
  // -print-before, and -stop-after options work.
  PassRegistry *Registry = PassRegistry::getPassRegistry();
  initializeCore(*Registry);
  initializeCodeGen(*Registry);
  initializeLoopStrengthReducePass(*Registry);
  initializeLowerIntrinsicsPass(*Registry);
  initializeUnreachableBlockElimPass(*Registry);


  cl::ParseCommandLineOptions(argc, argv, "llvm system compiler\n");


  /*  OwningPtr<tool_output_file> Out;  
  std::string ErrorInfo;
  Out.reset(new tool_output_file(OutputFilename.c_str(), ErrorInfo,
				 sys::fs::F_Binary));*/

  SMDiagnostic Err;
  std::auto_ptr<Module> M;
  M.reset(ParseIRFile(InputFilename, Err, Context));

  if (M.get() == 0) {
    Err.print(argv[0], errs());
    return 1;
  }


  Module &mod = *M.get();

  std::string Error;
  const std::string name("lc3");

  Triple TheTriple(M->getTargetTriple());
  if (TheTriple.getTriple().empty())
    TheTriple.setTriple(LLVMGetDefaultTargetTriple());

  /*for (TargetRegistry::iterator it = TargetRegistry::begin(),
         ie = TargetRegistry::end(); it != ie; ++it) {
    if (name == it->getName()) {
      TheTarget = &*it;
      break;
    }
    }*/

  const Target *TheTarget = TargetRegistry::lookupTarget(name, TheTriple,
                                                         Error);

  OwningPtr<tool_output_file> Out
    (GetOutputStream(TheTarget->getName(), TheTriple.getOS(), argv[0]));
  if (!Out) return 1;

  assert(TheTarget);

  TargetOptions options;

  options.PrintMachineCode = true;
  options.NoFramePointerElim = true;
  
  std::auto_ptr<TargetMachine>
    target(TheTarget->createTargetMachine(TheTriple.getTriple(),
                                          "", "", options,
                                          Reloc::Default, 
					  CodeModel::Default, CodeGenOpt::Default));

  assert(target.get() && "Could not allocate target machine!");
  TargetMachine &Target = *target.get();

  // Build up all of the passes that we want to do to the module.
  PassManager TargetPM;

  Target.addAnalysisPasses(TargetPM);

  if (const DataLayout *TD = Target.getDataLayout())
    TargetPM.add(new DataLayout(*TD));
  else
    TargetPM.add(new DataLayout(&mod));


  // Override default to generate verbose assembly.
  Target.setAsmVerbosityDefault(true);

  {
    formatted_raw_ostream FOS(Out->os());

    // Ask the target to add backend passes as necessary.
    if (Target.addPassesToEmitFile(TargetPM, FOS, 
				   TargetMachine::CGFT_AssemblyFile, true)) {
      errs() << argv[0] << ": target does not support generation of this"
             << " file type!\n";
      return 1;
    }

    TargetPM.run(*M.get());
  }


   
  //WriteBitcodeToFile(M.get(),Out->os());
   Out->keep();
   
   return 0;
}
