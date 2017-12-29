GCC_FLAGS = gcc -Wall -Wextra -g -Wvla -DNDEBUG -std=c99

all: HashIntSearch
	./HashIntSearch 1 0

GenericHashTable.o: GenericHashTable.h GenericHashTable.c TableErrorHandle.h
	$(GCC_FLAGS) -D MAX_ROW_ELEMENTS=2 -o GenericHashTable.o -c GenericHashTable.c

TableErrorHandle.o: TableErrorHandle.c
	$(GCC_FLAGS) -o TableErrorHandle.o -c TableErrorHandle.c

GenericHashTable: GenericHashTable.o TableErrorHandle.o Key.h
	ar rcs libgenericHashTable.a GenericHashTable.o TableErrorHandle.o

HashIntSearch.o: MyIntFunctions.h GenericHashTable.h HashIntSearch.c
	$(GCC_FLAGS) -o HashIntSearch.o -c HashIntSearch.c

MyIntFunctions.o: MyIntFunctions.h MyIntFunctions.c
	$(GCC_FLAGS) -o MyIntFunctions.o -c MyIntFunctions.c

HashIntSearch: HashIntSearch.o MyIntFunctions.o GenericHashTable
	gcc HashIntSearch.o MyIntFunctions.o -L. -lgenericHashTable -o HashIntSearch

HashStrSearch.o: MyStringFunctions.h GenericHashTable.h HashStrSearch.c
	$(GCC_FLAGS) -o HashStrSearch.o -c HashStrSearch.c

MyStrFunctions.o: MyStringFunctions.h MyStringFunctions.c
	$(GCC_FLAGS) -o MyStringFunctions.o -c MyStringFunctions.c

HashStrSearch: HashStrSearch.o MyStringFunctions.o GenericHashTable
	gcc HashStrSearch.o MyStringFunctions.o -L. -lgenericHashTable -o HashStrSearch

clean:
	rm -f GenericHashTable.o TableErrorHandle.o libgenericHashTable.a HashIntSearch.o MyIntFunctions.o HashIntSearch HashStrSearch.o MyStringFunctions.o HashStrSearch
