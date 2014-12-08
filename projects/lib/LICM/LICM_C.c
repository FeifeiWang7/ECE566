/* modified by fwang12, Feifei Wang*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvm-c/Core.h"
#include "dominance.h"
#include "cfg.h"
#include "loop.h"
#include "worklist.h"
#include "valmap.h"

static worklist_t list;
static LLVMBuilderRef Builder=NULL;

valmap_t calls, stores;
void *tmp;
unsigned int LICM_Count=0;  
unsigned int LICM_NoPreheader=0; 
unsigned int LICM_AfterLoop=0; 
unsigned int LICM_Load=0; 
unsigned int LICM_BadCall=0; 
unsigned int LICM_BadStore=0; 

int instInLoop(LLVMLoopRef L, LLVMValueRef addr)
{
	worklist_t worklist_bb = LLVMGetBlocksInLoop(L);
        LLVMBasicBlockRef bb_iter;
        for(bb_iter = (LLVMBasicBlockRef)worklist_pop(worklist_bb); bb_iter != NULL; bb_iter = (LLVMBasicBlockRef)worklist_pop(worklist_bb))
        {
                LLVMValueRef inst_iter;
                for(inst_iter = LLVMGetFirstInstruction(bb_iter); inst_iter != NULL; inst_iter = LLVMGetNextInstruction(inst_iter))
                {
			if(inst_iter == addr) return 1;
                }
	}
        return -1;
}

int noStoreToAddr(LLVMLoopRef L, LLVMValueRef addr)
{
	worklist_t worklist_bb = LLVMGetBlocksInLoop(L);
	LLVMBasicBlockRef bb_iter;
	for(bb_iter = (LLVMBasicBlockRef)worklist_pop(worklist_bb); bb_iter != NULL; bb_iter = (LLVMBasicBlockRef)worklist_pop(worklist_bb))
	{
		LLVMValueRef inst_iter;
                for(inst_iter = LLVMGetFirstInstruction(bb_iter); inst_iter != NULL; inst_iter = LLVMGetNextInstruction(inst_iter))
		{
			if(LLVMIsACallInst(inst_iter))
			{
				valmap_insert(calls, inst_iter, tmp);
				return -1;
			}
			if(LLVMIsAStoreInst(inst_iter) && (!LLVMIsAConstant(LLVMGetOperand(inst_iter, 1))) && (!LLVMIsAAllocaInst(LLVMGetOperand(inst_iter, 1)))) 
			{
				valmap_insert(stores, inst_iter, tmp);
				return -1;
			}
			if(LLVMIsAStoreInst(inst_iter) && (LLVMGetOperand(inst_iter, 1) == addr)) 
			{
				valmap_insert(stores, inst_iter, tmp);
				return -1;
			}
		}
	}
	return 1;
}

int noStore(LLVMLoopRef L)
{
	worklist_t worklist_bb = LLVMGetBlocksInLoop(L);
        LLVMBasicBlockRef bb_iter;
        for(bb_iter = (LLVMBasicBlockRef)worklist_pop(worklist_bb); bb_iter != NULL; bb_iter = (LLVMBasicBlockRef)worklist_pop(worklist_bb))
        {
		LLVMValueRef inst_iter;
                for(inst_iter = LLVMGetFirstInstruction(bb_iter); inst_iter != NULL; inst_iter = LLVMGetNextInstruction(inst_iter))
                {
			if(LLVMIsACallInst(inst_iter))
			{
                        	valmap_insert(calls, inst_iter, tmp);
                                return -1;
			}
			if(LLVMIsAStoreInst(inst_iter)) 
			{
				valmap_insert(stores, inst_iter, tmp);
                                return -1;
			}
		}
	}
	return 1;
}

int IDominatesExit(LLVMValueRef funs, LLVMLoopRef L, LLVMValueRef I)
{
	LLVMBasicBlockRef parent = LLVMGetInstructionParent(I);
	worklist_t exit_bb = LLVMGetExitBlocks(L);
	LLVMBasicBlockRef exit_iter;
	for(exit_iter = (LLVMBasicBlockRef)worklist_pop(exit_bb); exit_iter != NULL; exit_iter = (LLVMBasicBlockRef)worklist_pop(exit_bb))
	{
		if(!LLVMDominates(funs, parent, exit_iter)) return -1;
        }
	return 1;
}
int canMoveOutOfLoop(LLVMValueRef funs, LLVMLoopRef L, LLVMValueRef I, LLVMValueRef addr)
{
	if(LLVMIsAConstant(addr) && (noStoreToAddr(L, addr) == 1)) return 1;
	if((LLVMIsAAllocaInst(addr) && (instInLoop(L, addr) == -1)) && (noStoreToAddr(L, addr) == 1)) return 1;
	if((noStore(L) == 1) && (instInLoop(L, addr) == -1) && (IDominatesExit(funs, L, I)==1)) return 1;
	return -1;
}

int LICMOnFunction(LLVMValueRef funs)
{
	LLVMLoopInfoRef LI = LLVMCreateLoopInfoRef(funs);
	LLVMLoopRef Loop;
	for(Loop = LLVMGetFirstLoop(LI); Loop != NULL; Loop = LLVMGetNextLoop(LI, Loop))
	{
		calls = valmap_create(); //
		stores = valmap_create(); //
		LLVMBasicBlockRef PH = LLVMGetPreheader(Loop);
		if(PH == NULL) LICM_NoPreheader ++;
		else
		{
			LLVMValueRef PH_last;
			for(PH_last = LLVMGetFirstInstruction(PH); LLVMGetNextInstruction(PH_last) != NULL; PH_last = LLVMGetNextInstruction(PH_last)) ;
			
			worklist_t worklist_bb = LLVMGetBlocksInLoop(Loop);
			LLVMBasicBlockRef bb_iter;
			for(bb_iter = (LLVMBasicBlockRef)worklist_pop(worklist_bb); bb_iter != NULL; bb_iter = (LLVMBasicBlockRef)worklist_pop(worklist_bb))
			{
				LLVMValueRef inst_iter;
				for(inst_iter = LLVMGetFirstInstruction(bb_iter); inst_iter != NULL;)
				{
					LLVMValueRef tmpt = LLVMGetNextInstruction(inst_iter);
                                        if(LLVMMakeLoopInvariant(Loop, inst_iter)){
                                        	LICM_Count++;
						inst_iter = tmpt;
					}		
					else
					{
						if(LLVMIsALoadInst(inst_iter))
						{
							LLVMValueRef addr = LLVMGetOperand(inst_iter,0);
							if(canMoveOutOfLoop(funs, Loop, inst_iter, addr) == 1)
							{
								LICM_Count ++;
								LICM_Load ++;
								LLVMValueRef clone = LLVMCloneInstruction(inst_iter);
								LLVMPositionBuilderBefore(Builder, PH_last);
								LLVMInsertIntoBuilder(Builder, clone);
								LLVMReplaceAllUsesWith(inst_iter, clone);
								LLVMValueRef del = inst_iter;
								inst_iter = LLVMGetNextInstruction(inst_iter);
								LLVMInstructionEraseFromParent(del);
							} 
							else inst_iter = LLVMGetNextInstruction(inst_iter);
						}
						else inst_iter = LLVMGetNextInstruction(inst_iter);
					}	
				}
			}
		}
		LICM_BadCall = LICM_BadCall + valmap_size(calls);
		LICM_BadStore = LICM_BadStore + valmap_size(stores);
		valmap_destroy(calls);
		valmap_destroy(stores);
	}
	return 0;
}

void LoopInvariantCodeMotion_C(LLVMModuleRef Module)
{
	LLVMValueRef funs;

	Builder = LLVMCreateBuilder();

	list = worklist_create();

	for(funs=LLVMGetFirstFunction(Module);funs!=NULL;funs=LLVMGetNextFunction(funs))
	{ 
      		if (LLVMCountBasicBlocks(funs))
      			LICMOnFunction(funs);
    	}

	worklist_destroy(list);
  	LLVMDisposeBuilder(Builder);
  	Builder = NULL;

  fprintf(stderr,"LICM_Count      =%d\n",LICM_Count);
  fprintf(stderr,"LICM_NoPreheader=%d\n",LICM_NoPreheader);
  fprintf(stderr,"LICM_Load       =%d\n",LICM_Load);
  fprintf(stderr,"LICM_BadCall    =%d\n",LICM_BadCall);
  fprintf(stderr,"LICM_BadStore   =%d\n",LICM_BadStore);

}




