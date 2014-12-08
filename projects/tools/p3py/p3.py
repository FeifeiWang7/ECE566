#!/usr/bin/env python27

# watch out for uncollected objects
import gc

import sys, logging

from llvm import *
from llvm.core import *

from summary import *

if len(sys.argv) < 3:
    print "No enough arguments"
    exit(-1)

inFile = open(sys.argv[1],"rb")

M = Module.from_bitcode(inFile)

#for f in M.functions[:]:
#    for b in f.basic_blocks[:]:
#        for i in b.instructions[:]:
#            if i.is_terminator:
#                print "Found a terminator!"

Summarize(M,"noopt",sys.argv[1]+".noopt.stats")

outFile = open(sys.argv[2],"wb")

M.to_bitcode(outFile)


