LEVEL = ../..

vpath %.y $(SRC_DIR)
vpath %.lex $(SRC_DIR)

TOOLNAME=p1cpp

SOURCES := main.cpp parser.cpp scanner.cpp

LINK_COMPONENTS := bitreader bitwriter analysis 

include $(LEVEL)/Makefile.common

LIBS += -ll

parser.cpp: parser.y
	bison -d -o $@ $^

parser.hpp: parser.cpp

scanner.cpp: scanner.lex parser.hpp
	flex -o$@ $^
