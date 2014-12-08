
%.ll:%.bc
	llvm-dis $<

%.o:%.c
	gcc -c -o$@ $^ `llvm-config --cflags` 

%.o:%.cpp
	gcc -c -o$@ $^ `llvm-config --cxxflags` 

all: ex6

ex6: ex6.o valmap.o wrapper.o
	g++  -o$@ $^ `llvm-config --ldflags --libs` -lpthread -ldl

test: ex6
	clang -O0 -S -emit-llvm -o tmp.ll tmp.c
	opt -o tmp.bc tmp.ll
	./ex6 ./tmp.bc tmp.s

clean:
	rm -f *~ ex6 ex6.o tmp.bc tmp.ll tmp.s wrapper.o valmap.o
