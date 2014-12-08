#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvm-c/Core.h"
#include "dominance.h"
#include "summary.h"

typedef struct Stats_def {
  int functions;
  int globals;
  int bbs;
  int insns;
  int insns_bb_deps;
  int insns_g_deps;
  int branches;
  int loads;
  int stores;
  int calls;
  int loops; 
} Stats;

typedef struct loop_head {
	LLVMBasicBlockRef LoopHead;
	struct loop_head *next;
} Heads;
void pretty_print_stats(FILE *f, Stats s, int spaces)
{
  char spc[128];
  int i;

  for(i=0; i<spaces; i++)
    spc[i] = ' ';
  spc[i] = '\0';
    
  fprintf(f,"%sFunctions.......................%d\n",spc,s.functions);
  fprintf(f,"%sGlobal Vars.....................%d\n",spc,s.globals);
  fprintf(f,"%sBasic Blocks....................%d\n",spc,s.bbs);
  fprintf(f,"%sInstructions....................%d\n",spc,s.insns);
  fprintf(f,"%sInstructions (bb deps)..........%d\n",spc,s.insns_bb_deps);
  fprintf(f,"%sInstructions (g/c deps).........%d\n",spc,s.insns_g_deps);
  fprintf(f,"%sInstructions - Branches.........%d\n",spc,s.branches);
  fprintf(f,"%sInstructions - Loads............%d\n",spc,s.loads);
  fprintf(f,"%sInstructions - Stores...........%d\n",spc,s.stores);
  fprintf(f,"%sInstructions - Calls............%d\n",spc,s.calls);
  fprintf(f,"%sInstructions - Other............%d\n",spc,s.insns-s.branches-s.loads-s.stores);
  fprintf(f,"%sLoops...........................%d\n",spc,s.loops);
}

void print_csv_file(const char *filename, Stats s, const char *id)
{
  FILE *f = fopen(filename,"w");
  fprintf(f,"id,%s\n",id);
  fprintf(f,"functions,%d\n",s.functions);
  fprintf(f,"globals,%d\n",s.globals);
  fprintf(f,"bbs,%d\n",s.bbs);
  fprintf(f,"insns,%d\n",s.insns);
  fprintf(f,"insns_bb_deps,%d\n",s.insns_bb_deps);
  fprintf(f,"insns_g_deps,%d\n",s.insns_g_deps);
  fprintf(f,"branches,%d\n",s.branches);
  fprintf(f,"loads,%d\n",s.loads);
  fprintf(f,"stores,%d\n",s.stores);
  fprintf(f,"calls,%d\n",s.calls);
  fprintf(f,"loops,%d\n",s.loops);
  fclose(f);
}

Stats MyStats;
Heads HeadsStart;
void Summarize(LLVMModuleRef Module, const char *id, const char* filename)
{
	MyStats.functions = 0;
	MyStats.globals = 0;
	MyStats.bbs = 0;
	MyStats.insns = 0;
	MyStats.insns_bb_deps = 0;
	MyStats.insns_g_deps = 0;
	MyStats.branches = 0;
	MyStats.loads = 0;
	MyStats.stores = 0;
	MyStats.calls = 0;
	MyStats.loops = 0;

	LLVMValueRef global_iter;
	for(global_iter = LLVMGetFirstGlobal(Module); global_iter != NULL; global_iter = LLVMGetNextGlobal(global_iter))
	{
		if(LLVMGetInitializer(global_iter) != NULL) MyStats.globals ++;
	}	
	LLVMValueRef fn_iter;
	for(fn_iter = LLVMGetFirstFunction(Module); fn_iter != NULL; fn_iter = LLVMGetNextFunction(fn_iter))
	{
		if(LLVMGetFirstBasicBlock(fn_iter) != NULL ) MyStats.functions ++;
		LLVMBasicBlockRef bb_iter;
		for(bb_iter = LLVMGetFirstBasicBlock(fn_iter); bb_iter != NULL; bb_iter = LLVMGetNextBasicBlock(bb_iter))
		{
			MyStats.bbs ++;
			
			LLVMValueRef inst_iter;
			for(inst_iter = LLVMGetFirstInstruction(bb_iter); inst_iter != NULL; inst_iter = LLVMGetNextInstruction(inst_iter))
			{
				MyStats.insns ++;
				if(LLVMIsATerminatorInst(inst_iter) && !(LLVMIsAUnreachableInst(inst_iter))) 
				{
					MyStats.branches ++;
					if(LLVMIsABranchInst(inst_iter))
					{
						int j, m = LLVMGetNumOperands(inst_iter), flag = 0;
						for(j = 0; j < m; j++)
						{
							LLVMBasicBlockRef operandx;
							if(LLVMIsABasicBlock(LLVMGetOperand(inst_iter, j)))
							{
								operandx = (LLVMBasicBlockRef)LLVMGetOperand(inst_iter, j);
							}
							else continue;
							Heads *tmp = &HeadsStart;
							while(tmp->next != NULL)
							{
								tmp = tmp->next;
								if(tmp->LoopHead == operandx)
								{
									flag = 1;
									break;
								}
							}
							if((flag == 0) && LLVMDominates(fn_iter, operandx, bb_iter))
							{
								Heads *t = (Heads *)malloc(sizeof(Heads));
								t->LoopHead = operandx;
								t->next = NULL;
								tmp -> next = t;
								MyStats.loops ++;
							}
						}								
					}				
				}
				if(LLVMIsALoadInst(inst_iter)) MyStats.loads ++;
				if(LLVMIsAStoreInst(inst_iter)) MyStats.stores ++;
				if(LLVMIsACallInst(inst_iter)) MyStats.calls++;

				int n = LLVMGetNumOperands(inst_iter), i, global = 0, local = 0;
				for(i = 0; i < n; i++)
				{
					LLVMValueRef optr = LLVMGetOperand(inst_iter, i);
					if(LLVMIsAGlobalVariable(optr) || LLVMIsAConstant(optr)) global ++;
					else if(LLVMIsAInstruction(optr) && (LLVMGetInstructionParent(optr) == bb_iter)) local ++;
				}
				if(local > 0) MyStats.insns_bb_deps ++;
				else if((global == n)&& (n!=0)) MyStats.insns_g_deps++;	
			}
		}
	}
	print_csv_file(filename, MyStats, id);
}

