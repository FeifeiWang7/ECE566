/* modified by fwang12, Feifei Wang */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvm-c/Core.h"
#include "dominance.h"
#include "cfg.h"
#include "loop.h"
#include "worklist.h"
#include "valmap.h"

static LLVMBuilderRef Builder;

typedef struct VectorPairDef {
  LLVMValueRef pair[2];
  int insertAt0;
  struct VectorPairDef *next;
  struct VectorPairDef *prev;
	LLVMValueRef insertV;
} VectorPair;

typedef struct  {
  VectorPair *head;
  VectorPair *tail;
  valmap_t    visited;
  valmap_t    sliceA;
  int size;  
  int score;
} VectorList;

static VectorList* create() {
  VectorList *new = (VectorList*) malloc(sizeof(VectorList));
  new->head = NULL;
  new->tail = NULL;
  new->visited = valmap_create();
  new->sliceA = valmap_create();
  new->size=0;
 	new -> score = 65535;
  return new;
}

static void destroy(VectorList *list)
{
  valmap_destroy(list->visited);
  valmap_destroy(list->sliceA);
  VectorPair *head = list->head;
  VectorPair *tmp;
  while(head) {
    tmp = head;
    head=head->next;
    free(tmp);    
  }
  free(list);
}

static int dom(LLVMValueRef a, LLVMValueRef b)
{
  if (LLVMGetInstructionParent(a)!=LLVMGetInstructionParent(b)) {
    LLVMValueRef fun = LLVMGetBasicBlockParent(LLVMGetInstructionParent(a));
    return LLVMDominates(fun,LLVMGetInstructionParent(a), LLVMGetInstructionParent(b));
  }
  LLVMValueRef t = a;
  while(t!=NULL) {
    if (t==b) return 1;
    t = LLVMGetNextInstruction(t);
  }
  return 0;
}

static VectorPair *addPair(VectorList *list, LLVMValueRef a, LLVMValueRef b)
{
  VectorPair *new = (VectorPair*) malloc(sizeof(VectorPair));
  new->pair[0] = a;
  new->pair[1] = b;
  new->insertAt0 = 1;
  valmap_insert(list->visited,a,(void*)1);
  valmap_insert(list->visited,b,(void*)1);
  new->next = NULL;
  new->prev = NULL;
  if (list->head==NULL) {
    list->head = new;
    list->tail = new;
  } else {
    VectorPair *temp = list->head;
    VectorPair *prev = NULL;

    while(temp && dom(temp->pair[0],a)) {
      prev=temp;
      temp=temp->next;   
    }
    if (prev) {
      new->next = temp;
      new->prev = prev;
      prev->next = new;
      if (temp) temp->prev = new;
      else list->tail = new;
    } else {
      list->head->prev = new;
      new->next = list->head;
      list->head = new;
    }
  }  
  list->size++;
  return new;
}

int isTransformable(VectorList *list)
{
	VectorPair *vpair = list -> head;
	while(vpair != NULL)
	{
		LLVMValueRef I = vpair -> pair[0];
		LLVMValueRef J = vpair -> pair[1];

		LLVMValueRef vinst, usei, usej;
		int i, n = LLVMGetNumOperands(I);
		LLVMBasicBlockRef bb = LLVMGetInstructionParent(I);
		for(vinst = LLVMGetFirstInstruction(bb); vinst != NULL; vinst = LLVMGetNextInstruction(vinst))
		{
			usei = LLVMGetUsedValue(LLVMGetFirstUse(I));
			usej = LLVMGetUsedValue(LLVMGetFirstUse(J));
			if(dom(vinst, usei) && dom(vinst, usej))
			{
				for(i = 0; i < n; i ++)
				{
					if(!LLVMIsAInstruction(LLVMGetOperand(I, i))) continue;
					if(!LLVMIsAInstruction(LLVMGetOperand(J, i))) continue;
					if(dom(LLVMGetOperand(I, i), vinst) && dom(LLVMGetOperand(J, i), vinst)) ;
					else break;
				}
				if(i != n ) continue;
				else break;
			}
		}
		if(vinst == NULL) return -1;
		else
		{
			vpair->insertV = LLVMGetNextInstruction(vinst);
			vpair = vpair -> next;
		}
	}
	return 1;
}
static LLVMValueRef assembleVec2(LLVMValueRef a, LLVMValueRef b)
{
  LLVMTypeRef type = LLVMTypeOf(a);
  LLVMValueRef ret;

  if (LLVMIsAConstant(a) && LLVMIsAConstant(b)) {
    LLVMValueRef vec[2] = {a,b};
    ret = LLVMConstVector(vec,2);        
  }  else {
    LLVMTypeRef vtype = LLVMVectorType(type,2);
    LLVMValueRef c = LLVMConstNull(vtype);
    ret = LLVMBuildInsertElement(Builder,c,a, LLVMConstInt(LLVMInt32Type(),0,0),"v.ie");
    ret = LLVMBuildInsertElement(Builder,ret,b, LLVMConstInt(LLVMInt32Type(),1,0),"v.ie");    
  }
  return ret;
}
void printlist(VectorList *list)
{
	VectorPair *temp = list->head;
	while(temp)
	{
		LLVMDumpValue(temp->pair[0]);
		LLVMDumpValue(temp->pair[1]);
		printf("\n");
		temp = temp -> next;
	}	
}
int vectorize(VectorList *list)
{
	printlist(list);///
	valmap_t vmap = valmap_create();
	VectorPair *temp = list->head;
	while(temp && dom(temp->pair[0],temp->pair[1])) 
	{
		worklist_t ops = worklist_create();
		int i, n = LLVMGetNumOperands(temp->pair[0]);
		for(i = 0; i < n; i ++)
		{
			LLVMValueRef opi = LLVMGetOperand(temp->pair[0], i);
			LLVMValueRef opj = LLVMGetOperand(temp->pair[1], i);
			if((opi == NULL) || (opj == NULL)) printf("ATTENTION: the operands are null.\n");
			if(!valmap_check(vmap, opi))   
			{
				LLVMPositionBuilderBefore(Builder, temp->insertV);
				LLVMValueRef op = assembleVec2(opi, opj);
				worklist_insert(ops, op);
				valmap_insert(vmap, opi, op);
				valmap_insert(vmap, opj, op);
			}
			else 
			{
				LLVMValueRef op = valmap_find(vmap, opi);
				worklist_insert(ops, op);
			}
		}

		LLVMPositionBuilderBefore(Builder, temp->insertV); 
		LLVMValueRef new;
			
			LLVMValueRef op1, op2, op3;
			if(!worklist_empty(ops)) 
				op1 = worklist_pop(ops);
			if(!worklist_empty(ops)) op2 = worklist_pop(ops);
			if(!worklist_empty(ops)) op3 = worklist_pop(ops);
		switch(LLVMGetInstructionOpcode(temp->pair[0]))
		{
			 case LLVMAdd: 
			{
				new = LLVMBuildAdd(Builder, op2, op1, "v.add"); 
				break;
			}
			 case LLVMFAdd: new = LLVMBuildFAdd(Builder, op2, op1, "v.fadd"); break;
			 case LLVMSub: new = LLVMBuildSub(Builder, op2, op1, "v.sub"); break;
			 case LLVMFSub: new = LLVMBuildFSub(Builder, op2, op1, "v.fsub"); break;
			 case LLVMMul: new = LLVMBuildMul(Builder, op2, op1, "v.mul"); break;
			 case LLVMFMul: new = LLVMBuildFMul(Builder, op2, op1, "v.fmul"); break;
			 case LLVMUDiv: new = LLVMBuildUDiv(Builder, op2, op1, "v.udiv"); break;
			 case LLVMSDiv: new = LLVMBuildSDiv(Builder, op2, op1, "v.sdiv"); break;
			 case LLVMFDiv: new = LLVMBuildFDiv(Builder, op2, op1, "v.fdiv"); break;
			 case LLVMURem: new = LLVMBuildURem(Builder, op2, op1, "v.urem"); break;
			 case LLVMSRem: new = LLVMBuildSRem(Builder, op2, op1, "v.srem"); break;
			 case LLVMFRem: new = LLVMBuildFRem(Builder, op2, op1, "v.frem"); break;
			 case LLVMShl: new = LLVMBuildShl(Builder, op2, op1, "v.shl"); break;
			 case LLVMLShr: new = LLVMBuildLShr(Builder, op2, op1, "v.lshr"); break;
			 case LLVMAShr: new = LLVMBuildAShr(Builder, op2, op1, "v.ashr"); break;
			 case LLVMAnd: new = LLVMBuildAnd(Builder, op2, op1, "v.and"); break;
			 case LLVMOr: new = LLVMBuildOr(Builder, op2, op1, "v.or"); break;
			 case LLVMXor: new = LLVMBuildXor(Builder, op2, op1, "v.xor"); break;
			 case LLVMAlloca: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp->pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildAlloca(Builder, vtype, "v.alloca");
				break;
			}
			 case LLVMLoad: 
			{
				new = LLVMBuildLoad(Builder, op1, "v.load"); 
				break; 
			}
			 case LLVMStore: 
			{
				new = LLVMBuildStore(Builder, op2, op1); 
				break; 
			}
			/* case LLVMGetElementPtr:
			{
				printf("GEP...................\n"); 
				break;
			}*/
			 case LLVMTrunc: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp->pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildTrunc(Builder, op1, vtype, "v.trunc"); 
				break;
			}
			 case LLVMZExt:
			{
				LLVMTypeRef type = LLVMTypeOf(temp->pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildZExt(Builder, op1, vtype, "v.zext"); 
				break;
			}
			 case LLVMSExt: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp->pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildSExt(Builder, op1, vtype, "v.sext"); 
				break;
			}
			 case LLVMFPToUI: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp->pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildFPToUI(Builder, op1, vtype, "v.fptoui"); 
				break;
			}
			 case LLVMFPToSI: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp->pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildFPToSI(Builder, op1, vtype, "v.fptosi"); 
				break;
			}
			 case LLVMUIToFP: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp -> pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildUIToFP(Builder, op1, vtype, "v.uitofp"); 
				break;
			}
			 case LLVMSIToFP:
			{
				LLVMTypeRef type = LLVMTypeOf(temp -> pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildSIToFP(Builder, op1, vtype, "v.sitofp"); 
				break;
			}
			 case LLVMFPTrunc: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp -> pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildFPTrunc(Builder, op1, vtype, "v.fptrunc"); 
				break;
			}
			 case LLVMFPExt:
			{
				LLVMTypeRef type = LLVMTypeOf(temp -> pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildFPExt(Builder, op1, vtype, "v.fpext"); 
				break;
			}
			 case LLVMPtrToInt: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp -> pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildPtrToInt(Builder, op1, vtype, "v.ptrtoint"); 
				break;
			}
			 case LLVMIntToPtr: 
			{
				LLVMTypeRef type = LLVMTypeOf(temp -> pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildIntToPtr(Builder, op1, vtype, "v.inttoptr"); 
				break;
			}
			 case LLVMBitCast:
			{
				LLVMTypeRef type = LLVMTypeOf(temp -> pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildBitCast(Builder, op1, vtype, "v.bitcast"); 
				break;
			}
			 case LLVMSelect:
			{
				new = LLVMBuildSelect(Builder, op3, op2, op1, "");
				break;
			}
			 case LLVMVAArg:
			{
				LLVMTypeRef type = LLVMTypeOf(temp -> pair[0]);
				LLVMTypeRef vtype = LLVMVectorType(type, 2);
				new = LLVMBuildVAArg(Builder, op1, vtype, "v.vaarg"); 
				break;
			}
//			 case LLVMShuffleVector: printf("shufflevector.\n"); //new = LLVMBuildShuffleVector(Builder, worklist_pop(ops), worklist_pop(ops), worklist_pop(ops), ""); break;
			 //case LLVMExtractValue: printf("extractvalue.\n"); //new = LLVMBuildExtractValue(Builder, worklist_pop(ops), ops[1], ""); break;
			 //case LLVMInsertValue: printf("insertvalue.\n"); //new = LLVMBuildInsertValue(Builder, ], ops[1],ops[2], ""); break;
//			 case LLVMLandingPad: printf("LandingPad. \n"); //new = LLVMBuildLandingPad(Builder, ], ops[1], ops[2], ""); break;
			default: new = NULL; printf("Unused opcode ...............\n");
		}
		if(new != NULL)
		{
			valmap_insert(vmap, temp->pair[0], new);
			valmap_insert(vmap, temp->pair[1], new);
		}
		temp = temp->next;
	}
		
	temp = list -> head;
	while(temp)
	{
			
			LLVMValueRef vec = valmap_find(vmap, temp->pair[0]);

			if((LLVMGetInstructionOpcode(vec) == LLVMAlloca) || (LLVMGetInstructionOpcode(vec) == LLVMLoad))
			{
			//	LLVMValueRef evi = LLVMBuildGEP(Builder, LLVMValueRef Pointer, LLVMValueRef *Indices, unsigned NumIndices,"v.ee")
			}
			else
			{
				LLVMPositionBuilderBefore(Builder, LLVMGetNextInstruction(temp->pair[0]));
				LLVMValueRef evi = LLVMBuildExtractElement(Builder, vec, LLVMConstInt(LLVMInt32Type(),0,0), "v.ee");
				LLVMReplaceAllUsesWith(temp->pair[0], evi);
				LLVMInstructionEraseFromParent(temp->pair[0]);
				LLVMPositionBuilderBefore(Builder, LLVMGetNextInstruction(temp->pair[1]));
				LLVMValueRef evj = LLVMBuildExtractElement(Builder, vec, LLVMConstInt(LLVMInt32Type(),1,0), "v.ee");
				LLVMReplaceAllUsesWith(temp->pair[1], evj);
				LLVMInstructionEraseFromParent(temp->pair[1]);
			}
		temp = temp -> next;
	}
	//Remove any dead extractelements we inserted
}

int Isomorphic(LLVMValueRef I, LLVMValueRef J)
{
	if(LLVMGetInstructionOpcode(I) != LLVMGetInstructionOpcode(J)) return -1;
	if(LLVMTypeOf(I) != LLVMTypeOf(J)) return -1;
	int i, n = LLVMGetNumOperands(I), m = LLVMGetNumOperands(J);
	if(n != m) return -1;
	for(i = 0; i < n; i ++)
	{
		if(LLVMTypeOf(LLVMGetOperand(I, i)) != LLVMTypeOf(LLVMGetOperand(J, i))) return -1;
	} 
	return 1;
}
int isDependent(LLVMValueRef I, LLVMValueRef J)
{
	LLVMBasicBlockRef parentI = LLVMGetInstructionParent(I);
	int i, n = LLVMGetNumOperands(I);
	for(i = 0; i < n; i ++)
	{
		LLVMValueRef tmp = LLVMGetOperand(I, i);
		if(!LLVMIsAInstruction(tmp)) continue;
		if(tmp == J) return 1;
		else
		{
			if(LLVMGetInstructionParent(tmp) == parentI)
				return isDependent(tmp, J);
		}
	}
	return -1;
}

int shouldVectorize(LLVMValueRef I, LLVMValueRef J)
{
	if((!LLVMIsAInstruction(I)) || (!LLVMIsAInstruction(J))) return -1;
	if((LLVMGetTypeKind(LLVMTypeOf(I)) != LLVMIntegerTypeKind) && (LLVMGetTypeKind(LLVMTypeOf(I)) != LLVMHalfTypeKind) && (LLVMGetTypeKind(LLVMTypeOf(I)) != LLVMFloatTypeKind) && (LLVMGetTypeKind(LLVMTypeOf(I)) != LLVMDoubleTypeKind) && (LLVMGetTypeKind(LLVMTypeOf(I)) != LLVMX86_FP80TypeKind) && (LLVMGetTypeKind(LLVMTypeOf(I)) != LLVMFP128TypeKind) && (LLVMGetTypeKind(LLVMTypeOf(I)) != LLVMPPC_FP128TypeKind) && (LLVMGetTypeKind(LLVMTypeOf(I)) != LLVMPointerTypeKind)) 
	{
		return -1;
	}	
	if(LLVMGetInstructionParent(I) != LLVMGetInstructionParent(J))
	{
		return -1;
	}
	if(LLVMIsATerminatorInst(I))
	{
		return -1;
	}
	if((LLVMIsALoadInst(I) || LLVMIsAStoreInst(I)) && (LLVMGetVolatile(I)))
	{
		return -1;
	}
	if((LLVMIsALoadInst(J) || LLVMIsAStoreInst(J)) && (LLVMGetVolatile(J)))
	{
		return -1;
	}
	if(LLVMIsAPHINode(I) || LLVMIsACallInst(I) || LLVMIsAICmpInst(I) || LLVMIsAFCmpInst(I) || LLVMIsAExtractElementInst(I) || LLVMIsAInsertElementInst(I) || LLVMIsAAddrSpaceCastInst(I))
	{
		return -1;
	}
	if((LLVMGetInstructionOpcode(I) == LLVMAtomicCmpXchg) || (LLVMGetInstructionOpcode(I) == LLVMAtomicRMW))
	{
		return -1;
	}
	if(LLVMIsALoadInst(I))
	{
	/*	if((LLVMIsAAllocaInst(LLVMGetOperand(I,0)) || LLVMIsAGetElementPtrInst(LLVMGetOperand(I, 0))) && (((LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,0))) == LLVMIntegerTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,0))) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,0))) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,0))) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,0))) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,0))) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,0))) == LLVMPPC_FP128TypeKind)))) return 1;*/
		if((LLVMIsAAllocaInst(LLVMGetOperand(I,0)) || LLVMIsAGetElementPtrInst(LLVMGetOperand(I, 0))) && ((LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,0)))) == LLVMIntegerTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,0)))) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,0)))) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,0)))) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,0)))) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,0)))) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,0)))) == LLVMPPC_FP128TypeKind))) return 1; 
		else return -1;
	}
	if(LLVMIsAStoreInst(I)) 
	{
/*		if((LLVMIsAAllocaInst(LLVMGetOperand(I,1)) || LLVMIsAGetElementPtrInst(LLVMGetOperand(I, 1))) && (((LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,1))) == LLVMIntegerTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,1))) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,1))) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,1))) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,1))) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,1))) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(I,1))) == LLVMPPC_FP128TypeKind)))) return 1;	*/
		if((LLVMIsAAllocaInst(LLVMGetOperand(I,1)) || LLVMIsAGetElementPtrInst(LLVMGetOperand(I, 1))) && ((LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,1)))) == LLVMIntegerTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,1)))) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,1)))) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,1)))) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,1)))) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,1)))) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(I,1)))) == LLVMPPC_FP128TypeKind))) return 1; 
		else return -1;
	}

	if(LLVMIsALoadInst(J))
	{
	/*	if((LLVMIsAAllocaInst(LLVMGetOperand(J, 0)) || LLVMIsAGetElementPtrInst(LLVMGetOperand(J, 0))) && (((LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,0))) == LLVMIntegerTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,0))) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,0))) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,0))) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,0))) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,0))) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,0))) == LLVMPPC_FP128TypeKind)))) return 1;*/
		if((LLVMIsAAllocaInst(LLVMGetOperand(J,0)) || LLVMIsAGetElementPtrInst(LLVMGetOperand(J, 0))) && ((LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,0)))) == LLVMIntegerTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,0)))) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,0)))) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,0)))) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,0)))) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,0)))) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,0)))) == LLVMPPC_FP128TypeKind))) return 1;
		else return -1;
	}
	if(LLVMIsAStoreInst(J)) 
	{
	/*	if((LLVMIsAAllocaInst(LLVMGetOperand(J,1)) || LLVMIsAGetElementPtrInst(LLVMGetOperand(J, 1))) && (((LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,1))) == LLVMIntegerTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,1))) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,1))) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,1))) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,1))) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,1))) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(LLVMGetOperand(J,1))) == LLVMPPC_FP128TypeKind)))) return 1;	*/
		if((LLVMIsAAllocaInst(LLVMGetOperand(J,1)) || LLVMIsAGetElementPtrInst(LLVMGetOperand(J, 1))) && ((LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,1)))) == LLVMIntegerTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,1)))) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,1)))) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,1)))) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,1)))) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,1)))) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(LLVMGetOperand(J,1)))) == LLVMPPC_FP128TypeKind))) return 1;}
	if(isDependent(I,J) == 1) return -1;
////////////////////////////////////
	if(LLVMIsAAllocaInst(I))
	{
		LLVMTypeKind kind = LLVMGetTypeKind(LLVMGetElementType(LLVMTypeOf(I)));
		if((kind == LLVMVoidTypeKind) || (kind == LLVMHalfTypeKind) || (kind == LLVMFloatTypeKind) || (kind == LLVMDoubleTypeKind) || (kind == LLVMX86_FP80TypeKind) || (kind == LLVMFP128TypeKind) || (kind == LLVMPPC_FP128TypeKind) || (kind == LLVMIntegerTypeKind)) return 1;
		else return -1;
	}
	if(LLVMIsAGetElementPtrInst(I)) return -1;
	if(LLVMIsAExtractValueInst(I)) return -1;
	if(LLVMIsAInsertValueInst(I)) return -1;
	if(LLVMIsAShuffleVectorInst(I)) return -1;
	if(LLVMIsALandingPadInst(I)) return -1;
///////////////////////////////////////////////////
	return 1;
}

VectorList *collectIsomorphicInstructions(VectorList *L, LLVMValueRef I, LLVMValueRef J)
{
	if(shouldVectorize(I,J) == -1) return NULL;
	if(valmap_check(L->visited, I)) return L;
	if(valmap_check(L->visited, J)) return L;
	
	if(dom(I, J)) addPair(L, I, J);
	else if(dom(J, I)) addPair(L, J, I);
	int i, n = LLVMGetNumOperands(I);
	for(i = 0; i < n; i ++)
	{
		LLVMValueRef Itmp = LLVMGetOperand(I, i);
		LLVMValueRef Jtmp = LLVMGetOperand(J, i);
		if(LLVMIsAInstruction(Itmp) && LLVMIsAInstruction(Jtmp))
			if(Isomorphic(Itmp, Jtmp) == 1)
				collectIsomorphicInstructions(L, Itmp, Jtmp);
	}
	return L; ////new added
}

int calcScore(VectorList *list)
{
	int score = 0;
	
	VectorPair *vp;
	for(vp = list -> head; vp != NULL; vp = vp -> next)
	{
		LLVMValueRef I = vp -> pair[0];
		LLVMValueRef J = vp -> pair[1];
	
		if((LLVMGetTypeKind(LLVMTypeOf(I)) == LLVMHalfTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(I)) == LLVMFloatTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(I)) == LLVMDoubleTypeKind) || (LLVMGetTypeKind(LLVMTypeOf(I)) == LLVMX86_FP80TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(I)) == LLVMFP128TypeKind) || (LLVMGetTypeKind(LLVMTypeOf(I)) == LLVMPPC_FP128TypeKind)) score = score - 4;
		else score = score -1;
		LLVMUseRef use;
		for(use = LLVMGetFirstUse(I); use != NULL; use = LLVMGetNextUse(use))
		{
			LLVMValueRef user = LLVMGetUsedValue(use);
			if(!valmap_find(list->visited, user)) score ++;
		}
		for(use = LLVMGetFirstUse(J); use != NULL; use = LLVMGetNextUse(use))
                {
                        LLVMValueRef user = LLVMGetUsedValue(use);
                        if(!valmap_find(list->visited, user)) score ++;
                }
		int i, n = LLVMGetNumOperands(I);
		for(i = 0; i < n; i ++)
			if(!valmap_find(list->visited, LLVMGetOperand(I, i))) score++;
		for(i = 0; i < n; i ++)
                        if(!valmap_find(list->visited, LLVMGetOperand(J, i))) score++;
	}
	return score;
}


static void SLPOnBasicBlock(LLVMBasicBlockRef BB)
{
	LLVMValueRef I;
  	int changed;
  	int i=0;
  	do 
	{
    		changed = 0;
  		for(I=LLVMGetLastInstruction(BB); I!=NULL; I=LLVMGetPreviousInstruction(I))
    		{      
      			VectorList *list;
      			list = create();

			LLVMValueRef J;
			for(J = LLVMGetFirstInstruction(BB); J!=I; J = LLVMGetNextInstruction(J))
			{
				if(Isomorphic(I,J) == 1)
				{
					VectorList *tmplist;
					tmplist = create();	
					tmplist = collectIsomorphicInstructions(tmplist, I, J);
					if(tmplist != NULL)
					{
						if(tmplist -> size >= 2)
						{
							tmplist -> score = calcScore(tmplist);
							if(tmplist -> score <= list -> score) list = tmplist;
						}
						//else destroy(tmplist);
					}
				}
			}
			if(list -> size >= 2)
			{
				if(isTransformable(list) == 1)
				{
					printf("Size: %d\t", list->size);
printf("Begin vectorize................\n");
					vectorize(list);
					changed = 1;
					// update stats
					break;
				}
			}
    		}
		i++;
  	} while(changed && i<1); //changed from 3
}

static void SLPOnFunction(LLVMValueRef F) 
{
  LLVMBasicBlockRef BB;
  for(BB=LLVMGetFirstBasicBlock(F); BB!=NULL; BB=LLVMGetNextBasicBlock(BB)) SLPOnBasicBlock(BB);
}

void SLP_C(LLVMModuleRef Module)
{
	Builder = LLVMCreateBuilder();
  LLVMValueRef F;
  for(F=LLVMGetFirstFunction(Module); F!=NULL; F=LLVMGetNextFunction(F)) SLPOnFunction(F);
}
