OBJ_GEN = scanner.o parser.o ast.o ast-compile.o icode.o
OBJ_CORE = main.o
OBJ = $(OBJ_GEN) $(OBJ_CORE) 


HEADERS_CORE = error-display.hh user-options.hh ast.hh symbol-table.hh program.hh procedure.hh common-classes.hh icode.h 
HEADERS_GEN = parser.h parser.ih parserbase.h scanner.h scanner.ih scannerbase.h

HEADERS = $(HEADERS_CORE) $(HEADERS_GEN)

CPP = g++
BISONCPP = bisonc++
FLEXCPP = flexc++
TGT = sclp # c interpreter and compiler

option = -DCOMPILE
CFLAGS = -g

#change -lreference to -lreference14 for ubuntu14.04 in the target below

$(TGT): $(OBJ) 
	$(CPP) --std=c++0x -L. -Wl,-rpath=. $(option) $(OBJ) -o $(TGT) -lreference

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x $(option) $(CFLAGS) -c $<

scanner.cc scannerbase.h: scanner.ll scanner.ih scanner.h
	$(FLEXCPP) scanner.ll

parserbase.h: parser.cc

parser.cc: parser.yy parser.ih parser.h
	$(BISONCPP) --verbose parser.yy


clean :
	rm -rf test_files test_files_output
	rm -f *.o scannerbase.h parserbase.h scanner.cc parser.cc parser.yy.output
	rm -f sclp
	rm -f cscope.out tags a.out
