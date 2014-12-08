LLVMCONFIG=llvm-config

LLVMINCLUDE=$(shell $(LLVMCONFIG) --includedir)

LLVMLIBS=$(shell $(LLVMCONFIG) --libs)
LLVMLIBDIR=$(shell $(LLVMCONFIG) --libdir)

all:
	@bison -y -d -o calc.y.c calc.y
	@flex -ocalc.lex.c calc.lex
	@g++ $(shell $(LLVMCONFIG) --cflags --ldflags) -m64 -I$(LLVMINCLUDE) -L$(LLVMLIBDIR) -o calc main.c calc.y.c calc.lex.c -ll $(LLVMLIBS) -lpthread -ldl
	@echo [Built calc.]
clean:
	@rm -Rf calc.lex.c calc.y.c calc.y.h calc *~ *.bc *.ll
	@echo [Clean.]
