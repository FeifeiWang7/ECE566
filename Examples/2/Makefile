
%.lex.c:%.lex
	flex -o$@ $<

%.y.c:%.y
	bison -y -d -o$@ $<

part2: expr.y.c expr.lex.c 
	g++  -o$@ $^ `llvm-config --cflags --ldflags --libs` -ly -ll -lpthread -ldl

part1: expr.lex.c
	gcc -o$@ expr.lex.c -ll


clean:
	rm -f part1 part2 expr.lex.c expr.y.c expr.y.h *~
