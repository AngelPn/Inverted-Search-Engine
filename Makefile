#paths
INCL = include
SRC = src
MODULES = mylib
ODIR = build
TEST = tests

#compiler
CC = gcc

#compile options
CFLAGS = -Wall -g -I$(INCL) -I$(MODULES)/EntryList -I$(MODULES)/Entry -I$(MODULES)/BKTree

# OBJS =  $(SRC)/main.o
OBJS = $(MODULES)/Entry/Entry.o
OBJS += $(MODULES)/EntryList/EntryList.o
OBJS += $(MODULES)/BKTree/BKTree.o

all: test_Entry test_EntryList test_BKTree
	mkdir -p $(ODIR)
	mv $(OBJS) $(TEST)/Entry.test.o $(TEST)/EntryList.test.o $(TEST)/BKTree.test.o $(ODIR)

test_Entry: clean $(OBJS) $(TEST)/Entry.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/Entry.test.o -o test_Entry

test_EntryList: clean $(OBJS) $(TEST)/EntryList.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/EntryList.test.o -o test_EntryList

test_BKTree: clean $(OBJS) $(TEST)/BKTree.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/BKTree.test.o -o test_BKTree

# delete executable & object files
clean:
	rm -f test_Entry test_EntryList test_BKTree
	rm -rf $(ODIR)
