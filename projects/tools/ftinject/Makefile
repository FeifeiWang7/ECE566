##===- projects/sample/tools/sample/Makefile ---------------*- Makefile -*-===##

#
# Indicate where we are relative to the top of the source tree.
#
LEVEL=../..

#
# Give the name of the tool.
#
TOOLNAME=ftinject

#
# List libraries that we'll need
# We use LIBS because sample is a dynamic library.
#
USEDLIBS = summary.a GCM.a dominance.a FT.a cfg.a 

LINK_COMPONENTS := irreader bitreader bitwriter asmparser instrumentation scalaropts ipo vectorize

#
# Include Makefile.common so we know what to do.
#
include $(LEVEL)/Makefile.common

