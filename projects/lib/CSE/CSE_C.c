/* modified by fwang12, Feifei Wang */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvm-c/Core.h"
#include "dominance.h"
#include "transform.h"
#include "cfg.h"
#include "CSE.h"
static int commonSubexpression(LLVMValueRef I, LLVMValueRef J) 
{
	int num, i, flag = 0;
	if((LLVMGetInstructionOpcode(I) == LLVMGetInstructionOpcode(J)) && (LLVMTypeOf(I) == LLVMTypeOf(J)) && (LLVMGetNumOperands(I) == LLVMGetNumOperands(J)))
	{
		num = LLVMGetNumOperands(I);
		for( i = 0; i < num; i ++)
		{
			if(LLVMGetOperand(I, i) != LLVMGetOperand(J, i))
			{
				flag = 1;
				break;
			}
		}
		if(flag == 0) return 1;
	}
	return 0;
}

static int canHandle(LLVMValueRef I) 
{
  return !(LLVMIsALoadInst(I)||LLVMIsAExtractValueInst(I)||LLVMIsAStoreInst(I)||LLVMIsATerminatorInst(I)||LLVMIsACallInst(I)||LLVMIsAPHINode(I)||LLVMIsAAllocaInst(I)||LLVMIsAFCmpInst(I)||LLVMIsAVAArgInst(I));
}

int CSE_Basic = 0, CSE_Dead = 0, CSE_Simplify = 0, CSE_RLoad = 0, CSE_Store2Load = 0, CSE_RStore = 0;

int isDead(LLVMValueRef I)
{
  	if (LLVMGetFirstUse(I)!=NULL) return 0;
	LLVMOpcode opcode = LLVMGetInstructionOpcode(I);
  	switch(opcode) 
	{
  		case LLVMRet:
  		case LLVMBr:
  		case LLVMSwitch:
  		case LLVMIndirectBr:
  		case LLVMInvoke: 	
  		case LLVMUnreachable:
  		case LLVMFence:
  		case LLVMStore:
  		case LLVMCall:
  		case LLVMAtomicCmpXchg:
  		case LLVMAtomicRMW:
  		case LLVMResume:	
  		case LLVMLandingPad:	return 0;
  		case LLVMLoad: if(LLVMGetVolatile(I)) return 0;
  		default: break;
 	}
	return 1;
}
static void processInst(LLVMBasicBlockRef BB, LLVMValueRef I) 
{
	if(!canHandle(I)) return;       //not an optimizable instruction
        else
        {
                LLVMValueRef J;
                for(J = LLVMGetFirstInstruction(BB); J != NULL; )
                {
                        if(I == J) J = LLVMGetNextInstruction(J);
                        else if(commonSubexpression(I, J))
                        {
                                LLVMReplaceAllUsesWith(J, I);
                                LLVMValueRef rm = J;
                                J = LLVMGetNextInstruction(J);
                                LLVMInstructionEraseFromParent(rm);
                                CSE_Basic ++;
                        }
                        else J = LLVMGetNextInstruction(J);
                }
        }
        LLVMBasicBlockRef child;
        for(child = LLVMFirstDomChild(BB); child != NULL; child = LLVMNextDomChild(BB, child))
        {
                processInst(child, I);
        }
}

static void FunctionCSE(LLVMValueRef Function) 
{
	LLVMBasicBlockRef bb_iter;
	LLVMValueRef inst_iter;
	for(bb_iter = LLVMGetFirstBasicBlock(Function); bb_iter != NULL; bb_iter = LLVMGetNextBasicBlock(bb_iter))
		for(inst_iter = LLVMGetFirstInstruction(bb_iter); inst_iter != NULL; inst_iter = LLVMGetNextInstruction(inst_iter))
		{
			// begin simplify instruction
        		LLVMValueRef simpleInst = InstructionSimplify(inst_iter);
        		if(simpleInst)
        		{
                		LLVMReplaceAllUsesWith(inst_iter, simpleInst);
				LLVMValueRef rm = inst_iter;
                		inst_iter = LLVMGetPreviousInstruction(inst_iter);
                		LLVMInstructionEraseFromParent(rm);		
                		CSE_Simplify++;
        		}
			// *** end of simplify instruction
			// *** begin dead code elimination
			else if(isDead(inst_iter))
        		{
				LLVMValueRef rm = inst_iter;
                		inst_iter = LLVMGetPreviousInstruction(inst_iter);
                		LLVMInstructionEraseFromParent(rm);		
                		CSE_Dead ++;
        		}
			// *** end of dead code elimination
			else 
			{
				// *** CSE
				processInst(bb_iter, inst_iter);
				// *** end CSE
				// *** begin redundant load	
				if(LLVMGetInstructionOpcode(inst_iter) == LLVMLoad)
				{
					LLVMValueRef R = LLVMGetNextInstruction(inst_iter);
					while(R)
					{
						if((LLVMGetInstructionOpcode(R) == LLVMLoad) && (!LLVMGetVolatile(R)) && (LLVMGetOperand(inst_iter, 0) == LLVMGetOperand(R, 0)) && (LLVMTypeOf(R) == LLVMTypeOf(inst_iter)))
						{
							LLVMReplaceAllUsesWith(R, inst_iter);
							LLVMValueRef rm = R;
							R = LLVMGetNextInstruction(R);
							LLVMInstructionEraseFromParent(rm);
							CSE_RLoad++;
						}
						else if(LLVMGetInstructionOpcode(R) == LLVMStore) break; // no store to any address
						else if(LLVMIsACallInst(R)) break; // no call to any address
						else R = LLVMGetNextInstruction(R);
					}
				}
				// *** end redundant load
				// *** begin redundant store
				else if(LLVMGetInstructionOpcode(inst_iter) == LLVMStore)
				{
					LLVMValueRef R = LLVMGetNextInstruction(inst_iter);
					while(R)
					{
						if((LLVMGetInstructionOpcode(R) == LLVMLoad) && (!LLVMGetVolatile(R)) && (LLVMGetOperand(inst_iter, 1) == LLVMGetOperand(R, 0)) && (LLVMTypeOf(R) == LLVMTypeOf(LLVMGetOperand(inst_iter, 0))))
						{
							LLVMReplaceAllUsesWith(R, LLVMGetOperand(inst_iter, 0));
							LLVMValueRef rm = R;
							R = LLVMGetNextInstruction(R);
							LLVMInstructionEraseFromParent(rm);
							CSE_Store2Load++;
							continue;
						}
						else if((LLVMGetInstructionOpcode(R) == LLVMStore) && (!LLVMGetVolatile(R)) && (LLVMGetOperand(inst_iter, 1) == LLVMGetOperand(R, 1)) && (LLVMTypeOf(LLVMGetOperand(inst_iter, 0)) == LLVMTypeOf(LLVMGetOperand(R, 0))))
						{
							LLVMValueRef rm = inst_iter;
							inst_iter = LLVMGetPreviousInstruction(inst_iter);
							LLVMInstructionEraseFromParent(rm);
							CSE_RStore++;
							break;			
						}	
						else if((LLVMGetInstructionOpcode(R) == LLVMStore) || (LLVMGetInstructionOpcode(R) == LLVMLoad)) break;
						else if(LLVMIsACallInst(R)) break; // no call to any address
						else R = LLVMGetNextInstruction(R);
					}
				}
				// *** end redundant store
			}
		}
}

void LLVMCommonSubexpressionElimination(LLVMModuleRef Module)
{
	LLVMValueRef Function;
	for (Function=LLVMGetFirstFunction(Module);Function!=NULL;Function=LLVMGetNextFunction(Function)) FunctionCSE(Function);
	printf("Total\t CSE_Basic\t CSE_Dead\t CSE_Simplify\t CSE_RLoad\t CSE_Store2Load\t CSE_RStore\n");
	printf("%d\t\t %d\t\t %d\t\t %d\t\t %d\t\t %d\t\t %d\n", CSE_Basic+CSE_Dead+CSE_Simplify+CSE_RLoad+CSE_Store2Load+CSE_RStore, CSE_Basic, CSE_Dead, CSE_Simplify, CSE_RLoad, CSE_Store2Load, CSE_RStore);
}

