
%.ll:%.bc
	llvm-dis $<

%.o:%.c
	gcc -c -o$@ $^ `llvm-config --cflags` 

ex4: ex4.o
	g++  -o$@ $^ `llvm-config --cflags --ldflags --libs` -lpthread -ldl

clean:
	rm -f part1 part2 expr.lex.c expr.y.c expr.y.h *~
