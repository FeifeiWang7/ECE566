/*
 * File: Summary.cpp
 *
 * Description:
 *   This is where you implement your project 3 support.
 */

#include <stdio.h>
#include <stdlib.h>

/* LLVM Header Files */
//#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Type.h"

/* Header file global to this project */
#include "Summary.hpp"

using namespace llvm;

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
  int loops; //approximated by backedges
} Stats;

void pretty_print_stats(FILE *f, Stats s, int spaces)
{
  char spc[128];
  int i;

  // insert spaces before each line
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
  fprintf(f,"%sInstructions - Other............%d\n",spc,
	  s.insns-s.branches-s.loads-s.stores);
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

void
Summarize_Cpp(Module *M, const char *id, const char* filename)
{
  // summarize file contents and dump to filename


}

