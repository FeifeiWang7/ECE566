
%.ll:%.bc
	llvm-dis $<

%.o:%.c
	gcc -c -o$@ $^ `llvm-config --cflags` 

%.o:%.cpp
	gcc -c -o$@ $^ `llvm-config --cxxflags` 

all: ex5 dce

ex5: ex5.o 
	g++  -o$@ $^ `llvm-config --ldflags --libs` -lpthread -ldl

dce: dce.o worklist.o
	g++  -o$@ $^ `llvm-config --ldflags --libs` -lpthread -ldl

test: dce
	clang -O0 -S -emit-llvm -o tmp.ll tmp.c
	opt -mem2reg -o tmp.bc tmp.ll
	./dce ./tmp.bc tmp-dce.bc
	llvm-dis tmp-dce.bc

clean:
	rm -f ex5 dce dce.o gepmap.o ex5.o worklist.o *~ *.ll *.bc
