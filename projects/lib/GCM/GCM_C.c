/*modified by fwang12, Feifei Wang*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvm-c/Core.h"
#include "dominance.h"
#include "cfg.h"

static LLVMBuilderRef Builder;
LLVMBasicBlockRef GetEarly(LLVMValueRef F, LLVMValueRef I)
{
        LLVMBasicBlockRef Early = LLVMGetInstructionParent(I);
        LLVMBasicBlockRef BEarly = LLVMImmDom(Early);
        while(BEarly)
        {
                int i, n = LLVMGetNumOperands(I);
                for(i = 0; i < n; i ++)
                {
                        LLVMValueRef operand = LLVMGetOperand(I, i);
                        if(LLVMIsAInstruction(operand))
                        {
                                LLVMBasicBlockRef opbb = LLVMGetInstructionParent(operand);
                                if(!LLVMDominates(F, opbb, BEarly)) return Early;
                        }
                }
                Early = BEarly;
                BEarly = LLVMImmDom(BEarly);
        }
        return Early;
}
LLVMBasicBlockRef GetLate(LLVMValueRef F, LLVMValueRef I)
{
        LLVMBasicBlockRef Late;
        LLVMUseRef tmpa = LLVMGetFirstUse(I);
        if(tmpa == NULL) return LLVMGetInstructionParent(I); /// should be the dominance last block???
        LLVMValueRef tmpb = LLVMGetUsedValue(tmpa);
        if(LLVMIsAPHINode(tmpb))
        {
                int i, n = LLVMCountIncoming(tmpb);
                for(i = 0; i < n; i ++)
                {
                        if (LLVMGetOperand(tmpb, i) == LLVMGetUser(tmpa))
                        {
                                printf("here.\n");
                                break;
                        }
                }
                Late = LLVMGetIncomingBlock(tmpb, i);
        }
        else Late = LLVMGetInstructionParent(tmpb);
        for(LLVMUseRef use = LLVMGetNextUse(tmpa); use != NULL; use = LLVMGetNextUse(use))
        {
                LLVMValueRef tmpc = LLVMGetUsedValue(use);
                if(LLVMIsAPHINode(tmpc))
                {
                        int i, n = LLVMCountIncoming(tmpc);
                        for(i = 0; i < n; i ++)
                        {
                                if(LLVMGetOperand(tmpc, i) == LLVMGetUser(use))
                                {
                                        printf("here too.\n");
                                        break;
                                }
                        }
                        Late = LLVMNearestCommonDominator(Late, LLVMGetIncomingBlock(tmpc, i));
                }
                else Late = LLVMNearestCommonDominator(Late, LLVMGetInstructionParent(tmpc));
        }
        return Late;
}
void GlobalCodeMotion_C(LLVMModuleRef Module)
{
        int GCM_Count = 0;
        double GCM_AvgDistance = 0;
        int GCM_AfterLoop = 0;
        int GCM_BeforeLoop = 0;

        Builder = LLVMCreateBuilder();
        LLVMValueRef F;
        for(F= LLVMGetFirstFunction(Module); F!=NULL; F = LLVMGetNextFunction(F))
        {
                LLVMBasicBlockRef BB;
                for(BB=LLVMGetFirstBasicBlock(F); BB!=NULL; BB=LLVMGetNextBasicBlock(BB))
                {
                        LLVMValueRef I;
                        for(I = LLVMGetFirstInstruction(BB); I != NULL; I = LLVMGetNextInstruction(I))
                        {
                                if(LLVMIsALoadInst(I) || LLVMIsAStoreInst(I) || LLVMIsACallInst(I) || LLVMIsAInvokeInst(I) || LLVMIsAPHINode(I) || LLVMIsATerminatorInst(I) || LLVMIsAAllocaInst(I))
                                {
                                //      I = LLVMGetNextInstruction(I);
                                        continue;
                                }
                                LLVMBasicBlockRef Early = GetEarly(F, I);
                                LLVMBasicBlockRef Late = GetLate(F, I);
                                LLVMBasicBlockRef Best = Late;
                                int flag = 0;
                                while(Late != Early)
                                {
                                        Late = LLVMImmDom(Late);
                                        if(LLVMGetLoopNestingDepth(Late) < LLVMGetLoopNestingDepth(Best))
                                        {
                                                Best = Late;
                                                flag = 1;
                                        }
                                }
				if(flag == 1)
                                {
                                        GCM_Count++;
                                        LLVMBasicBlockRef tt;
                                        if(LLVMDominates(F, Best, BB))
                                        {
                                                for(tt = Best; tt != BB; tt = LLVMGetNextBasicBlock(tt)) GCM_AvgDistance++;
                                                GCM_BeforeLoop++;
                                        }
                                        else
                                        {
                                                for(tt = BB; tt != Best; tt = LLVMGetNextBasicBlock(tt)) GCM_AvgDistance++;
                                                GCM_AfterLoop++;
                                        }
                                }
                                //place I in best
                        /*      LLVMValueRef clone = LLVMCloneInstruction(I);
                                LLVMPositionBuilderBefore(Builder, LLVMGetLastInstruction(Best));
                                LLVMInsertIntoBuilder(Builder, clone);
                                LLVMReplaceAllUsesWith(I, clone);
                                LLVMValueRef del = I;
                                I = LLVMGetNextInstruction(I);
                                LLVMInstructionEraseFromParent(del);*/
                        }
                }
        }
        if(GCM_Count == 0) GCM_AvgDistance = 0;
        else GCM_AvgDistance = GCM_AvgDistance/GCM_Count;
        printf("GCM_Count:\t %d\nGCM_AvgDistance: %.2lf\nGCM_AfterLoop:\t %d\nGCM_BeforeLoop:\t %d\n", GCM_Count, GCM_AvgDistance, GCM_AfterLoop, GCM_BeforeLoop);
}



