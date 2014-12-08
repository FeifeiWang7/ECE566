##===- projects/sample/tools/sample/Makefile ---------------*- Makefile -*-===##

#
# Indicate where we are relative to the top of the source tree.
#
LEVEL=../..

#
# Give the name of the tool.
#
TOOLNAME=lc3as

#
# List libraries that we'll need
# We use LIBS because sample is a dynamic library.
#
USEDLIBS = summary.a SummaryCpp.a CSE.a GCM.a dominance.a cfg.a transform.a LC3.a LC3Desc.a LC3Info.a

LINK_COMPONENTS := all codegen irreader bitwriter core 

#
# Include Makefile.common so we know what to do.
#
include $(LEVEL)/Makefile.common

