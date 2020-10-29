CC=gcc

all: compile_all

debug: debug_all

debug_all: driver.c datastructuresandprototypes.h readGrammar.c tokeniseSourcecode.c createParseTree.c traverseParseTree.c printParseTree.c printTypeExpressionTable.c freeUtilities.c
	gcc -g -o output driver.c readGrammar.c tokeniseSourcecode.c createParseTree.c traverseParseTree.c printParseTree.c printTypeExpressionTable.c freeUtilities.c
	
compile_all: driver.c datastructuresandprototypes.h readGrammar.c tokeniseSourcecode.c createParseTree.c printParseTree.c printParseTree.c printTypeExpressionTable.c freeUtilities.c
	gcc -o output driver.c readGrammar.c tokeniseSourcecode.c createParseTree.c traverseParseTree.c printParseTree.c printTypeExpressionTable.c freeUtilities.c



# object files, if needed in future

driver.o: driver.c
	gcc -c driver.c

readGrammer.o: readGrammer.c
	gcc -c readGrammer.c

tokeniseSourcecode.o: tokeniseSourcecode.c
	gcc -c readGrammer.c

createParseTree.o: createParseTree.c
	gcc -c createParseTree.c

traverseParseTree.o: traverseParseTree.c
	gcc -c traverseParseTree.c

printParseTree.o: printParseTree.c
	gcc -c printParseTree.c

printTypeExpressionTable.o: printTypeExpressionTable.c
	gcc -c printTypeExpressionTable.c

clean:
	rm *.o
