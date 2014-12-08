#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

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





using namespace llvm;
#include "FT.h"
#include "Summary.hpp"

#include <time.h>

static cl::opt<std::string>
InputFilename(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));

static cl::opt<std::string>
OutputFilename("o", cl::desc("Output filename"), cl::value_desc("filename"));


static cl::opt<bool>
NoFlowInject("no-flow-errors",
  cl::desc("Inject control flow errors."),
  cl::init(false));

static cl::opt<bool>
NoDataInject("no-data-errors",
  cl::desc("Inject data flow errors."),
  cl::init(false));

static cl::opt<unsigned>
NumErrors("inject-count",
  cl::desc("Inject num errors."),
  cl::init(1));

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

int
main (int argc, char ** argv)
{
  cl::ParseCommandLineOptions(argc, argv, "llvm system compiler\n");

  char filename[1024];
  if (argc < 3) {
    fprintf(stderr,"Not enough positional arguments to %s.\n",argv[0]);
    return 1;
  }

  srand(time(NULL));

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

  // If we don't yet have an output filename, make one.
  if (OutputFilename.empty()) {
    if (InputFilename == "-")
      OutputFilename = "-";
    else
      {
	OutputFilename = GetFileNameRoot(InputFilename);	
	OutputFilename+=".1.bc";
      }
  }

  Module::iterator fit, fend;
  Function::iterator bit, bend;
  BasicBlock::iterator it, iend;

  int ctrlInjCnt=0;
  int dataInjCnt=0;

  Module *m = M.get();

  // Loop until we have injected enough errors.  
  // Based on simple probability, we should eventually escape,
  // but, may need to revisit for a few possible pathological cases
  // that will trigger an infinite loop
  while (ctrlInjCnt+dataInjCnt < NumErrors)
    {
      
      for(fit=m->begin(), fend=m->end(); fit!=fend; fit++)
	{
	  Function *F = fit;
	  
	  if(!F->empty())
	    {
	      for(bit=F->begin(),bend=F->end(); bit!=bend; bit++)
		{
		  BasicBlock *BB = bit;
		  
		  for (it=BB->begin(),iend=BB->end(); it!=iend; it++)
		    {
		      Instruction *I = it;
		      int r = rand() % 1023;

		      // pick an instruction with a certain probability. This
		      // helps guarantee we don't pick the same instruction every time
		      if (r < 20) {
			
			// If we are allowed to inject control flow errors
			if (!NoFlowInject)
			  {
			    if( isa<BranchInst>(I) )
			      {
				BranchInst *BI = (BranchInst*)I;
				
				if (BI->isConditional())
				  {
				    Value *V = BI->getCondition();
				    // Invert the condition
				    BinaryOperator *BO = 
				      BinaryOperator::Create(Instruction::Xor,V,ConstantInt::get(V->getType(),1),"fi_inv",I);
				    BI->setCondition(BO);
				    ctrlInjCnt++;
				    continue;
				  }			    
			      }
			  }

			// If we are allowed to inject data errors
			if (!NoDataInject && !isa<BranchInst>(I))
			  {
			    // Find an integer operand
			    unsigned i;
			    for(i=0; i<I->getNumOperands(); i++)
			      {
				Value *v = I->getOperand(i);
				if (v->getType()->isIntegerTy())
				  {
				    // inject a 0 or 1 based on random number
				    // note: we may not actually change the behavior
				    // if we replace with the same value.
				    // Since it's probabilistic anyway, no reason to 
				    // check.
				    if (rand()%100 < 50)
				      I->setOperand(i,ConstantInt::get(v->getType(),0));
				    else
				      I->setOperand(i,ConstantInt::get(v->getType(),1));
				    
				    dataInjCnt++;
				    break;
				  }
				
			      } 
			  }     
			    
		      }
		    }
		}	  
	    }
	} 
    }


  WriteBitcodeToFile(M.get(),Out->os());

  Out->keep();

  return 0;
}
