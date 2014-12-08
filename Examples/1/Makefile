
%.lex.c:%.lex
	flex -o$@ $<

%.y.c:%.y
	bison -y -d -o$@ $<

part1: expr.lex.c
	gcc -o$@ expr.lex.c -ll

part2: expr.y.c expr.lex.c 
	gcc -o$@ $^ -ly -ll

clean:
	rm -f part1 part2 expr.lex.c expr.y.c expr.y.h *~
