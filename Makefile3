# Paths
INCL = include
SRC = src
MODULES = mylib
ODIR = build
TEST = tests

# Compiler
CC = gcc

# Compile options
CFLAGS = -Wall -g -I$(INCL) -I$(MODULES)/EntryList -I$(MODULES)/Entry -I$(MODULES)/BKTree -I$(MODULES)/HammingTree
# Valgrind flags
VALFLAGS = --leak-check=full --track-origins=yes -s

OBJS = $(MODULES)/Entry/Entry.o
OBJS += $(MODULES)/EntryList/EntryList.o
OBJS += $(MODULES)/BKTree/BKTree.o

all: test_Entry test_EntryList test_BKTree
	mkdir -p $(ODIR)
	mv $(OBJS) $(TEST)/Entry.test.o $(TEST)/EntryList.test.o $(TEST)/BKTree.test.o $(ODIR)

mainonly: clean $(OBJS) $(MODULES)/HashTable/HashTable.o $(MODULES)/HammingTree/HammingTree.o $(MODULES)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(MODULES)/HashTable/HashTable.o $(MODULES)/HammingTree/HammingTree.o $(MODULES)/main.o -o main
	mkdir -p $(ODIR)
	mv $(OBJS) $(MODULES)/HashTable/HashTable.o $(MODULES)/HammingTree/HammingTree.o $(MODULES)/main.o $(ODIR)

move:
	mkdir -p $(ODIR)
	mv $(OBJS) $(ODIR)

test_Entry: clean $(OBJS) $(TEST)/Entry.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/Entry.test.o -o test_Entry

test_EntryList: clean $(OBJS) $(TEST)/EntryList.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/EntryList.test.o -o test_EntryList

test_BKTree: clean $(OBJS) $(TEST)/BKTree.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/BKTree.test.o -o test_BKTree

run: all
	./test_Entry
	./test_EntryList
	./test_BKTree

valgrind: all
	valgrind $(VALFLAGS) ./test_Entry
	valgrind $(VALFLAGS) ./test_EntryList
	valgrind $(VALFLAGS) ./test_BKTree

# Delete executable & object files
clean:
	rm -f test_Entry test_EntryList test_BKTree main
	rm -rf $(ODIR)
