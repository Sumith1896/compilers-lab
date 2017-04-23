AST_OBJ = ast.o ast-compile.o
PROG_OBJ = program.o program-compile.o
PROC_OBJ = procedure.o procedure-compile.o
SYMTAB_OBJ = symbol-table.o symbol-table-compile.o
OBJ_CORE = error-display.o main.o user-options.o icode.o reg-alloc.o
OBJ_GEN = scanner.o parser.o

HEADERS_CORE = error-display.hh user-options.hh ast.hh symbol-table.hh program.hh procedure.hh icode.hh reg-alloc.hh common-classes.hh
HEADERS_GEN = parser.h parser.ih parserbase.h scanner.h scanner.ih scannerbase.h

HEADERS = $(HEADERS_CORE) $(HEADERS_GEN)
OBJ = $(OBJ_GEN) $(OBJ_CORE) $(AST_OBJ) $(PROC_OBJ) $(PROG_OBJ) $(BB_OBJ) $(SYMTAB_OBJ)

CPP = g++
BISONCPP = bisonc++
FLEXCPP = flexc++
TGT = sclp

option = -DCOMPILE
CFLAGS = -g

$(TGT): $(OBJ) 
	$(CPP) --std=c++0x $(option) $(OBJ) -pg -o $(TGT)

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x $(option) $(CFLAGS) -c $<

scanner.cc scannerbase.h: scanner.ll scanner.ih scanner.h
	$(FLEXCPP) scanner.ll

parser.cc parserbase.h: parser.yy parser.ih parser.h
	$(BISONCPP) --verbose parser.yy

clean :
	rm -f *.o scannerbase.h parserbase.h scanner.cc parser.cc parser.yy.output
	rm -f sclp
	rm -f cscope.out tags a.out
