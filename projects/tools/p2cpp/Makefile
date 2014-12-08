LEVEL = ../..

vpath %.y $(SRC_DIR)
vpath %.lex $(SRC_DIR)

TOOLNAME=p2cpp

SOURCES := main.cpp minic.y.cpp minic.lex.cpp Cmdline.cpp list.cpp symbol.cpp

LINK_COMPONENTS := bitreader bitwriter analysis 

include $(LEVEL)/Makefile.common

LIBS += -lfl

minic.y.cpp: minic.y
	bison -d -o $@ $^

minic.y.hpp: minic.y.cpp

minic.lex.cpp: minic.lex minic.y.hpp
	flex -o$@ $^

list.cpp: list.h
symbol.cpp: symbol.h

Cmdline.cpp: cmdline.lex
	flex -PCmd -o$@ cmdline.lex 
