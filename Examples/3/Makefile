
%.ll:%.bc
	llvm-dis $<

%.lex.c:%.lex
	flex -o$@ $<

%.y.c:%.y
	bison -y -d -o$@ $<

expr: expr.y.c expr.lex.c 
	g++  -o$@ $^ `llvm-config --cflags --ldflags --libs` -ly -ll -lpthread -ldl

clean:
	rm -f part1 part2 expr.lex.c expr.y.c expr.y.h *~
