#!/usr/bin/env python27

# watch out for uncollected objects
import gc

import sys, logging

from llvm import *
from llvm.core import *

inFile = open(sys.argv[1],"rb")

M = Module.from_bitcode(inFile)

#for f in M.functions[:]:
#    for b in f.basic_blocks[:]:
#        for i in b.instructions[:]:
#            if i.is_terminator:
#                print "Found a terminator!"

outFile = open(sys.argv[2],"wb")

M.to_bitcode(outFile)


