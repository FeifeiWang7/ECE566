
%.ll:%.bc
	llvm-dis $<

%.o:%.c
	gcc -c -o$@ $^ `llvm-config --cflags` 

%.o:%.cpp
	gcc -c -o$@ $^ `llvm-config --cxxflags` 

sra: sra.o gepmap.o worklist.o
	g++  -o$@ $^ `llvm-config --ldflags --libs` -lpthread -ldl

test:
	clang -O0 -S -emit-llvm -o tmp.ll tmp.c
	opt -mem2reg -o tmp.bc tmp.ll
	./sra ./tmp.bc tmp-sra.bc
	llvm-dis tmp-sra.bc

clean:
	rm -f sra gepmap.o sra.o worklist.o *~ *.ll *.bc tmp-*.bc tmp-*.ll
