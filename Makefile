# Copyright (c) 2013 KAUST - InfoCloud Group (All Rights Reserved)
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# Authors: Amin Allam  <amin.allam@kaust.edu.sa>,
#          Fuad Jamour <fuad.jamour@kaust.edu.sa>
#
# Current version: 1.0 (initial release)

# Paths
INCL = include
MODULES = mylib
ODIR = build
TEST = tests
	
TEST_O = test_driver/test.o
IMPL_O = ref_impl/core.o
MAIN_O = ref_impl/main.o
OBJS = $(MODULES)/Index/Index.o $(MODULES)/Entry/Entry.o $(MODULES)/Query/Query.o
OBJS += $(MODULES)/LinkedList/LinkedList.o $(MODULES)/BKTree/BKTree.o
OBJS += $(MODULES)/HashTable/HashTable.o $(MODULES)/HammingTree/HammingTree.o
OBJS += $(MODULES)/Document/Document.o 

# Compilers
CC  = gcc
CXX = g++

# Compile flags
CFLAGS = -O3 -fPIC -mavx2 -Wall -g -I$(INCL) -I$(MODULES)/Entry -I$(MODULES)/Index -I$(MODULES)/Query -I$(MODULES)/LinkedList -I$(MODULES)/HashTable -I$(MODULES)/BKTree -I$(MODULES)/HammingTree -I$(MODULES)/Document
CXXFLAGS = $(CFLAGS)
LDFLAGS = -lpthread
# Valgrind flags
VALFLAGS = --leak-check=full --track-origins=yes -s

PROGRAMS = test_Entry test_LinkedList test_BKTree test_HammingTree test_Query test_Document test_HashTable testdriver
LIBRARY = core

all: $(PROGRAMS)
	mkdir -p $(ODIR)
	mv $(OBJS) $(IMPL_O) $(TEST_O) $(TEST)/Entry.test.o $(TEST)/LinkedList.test.o $(TEST)/BKTree.test.o $(TEST)/HammingTree.test.o $(TEST)/Query.test.o $(TEST)/Document.test.o $(TEST)/HashTable.test.o $(ODIR)

test_Entry: clean $(IMPL_O) $(OBJS) $(TEST)/Entry.test.o
	$(CC) $(CFLAGS) $(IMPL_O) $(OBJS) $(TEST)/Entry.test.o -o test_Entry

test_LinkedList: clean $(IMPL_O) $(OBJS) $(TEST)/LinkedList.test.o
	$(CC) $(CFLAGS) $(IMPL_O) $(OBJS) $(TEST)/LinkedList.test.o -o test_LinkedList

test_BKTree: clean $(IMPL_O) $(OBJS) $(TEST)/BKTree.test.o
	$(CC) $(CFLAGS) $(IMPL_O) $(OBJS) $(TEST)/BKTree.test.o -o test_BKTree

test_HammingTree: clean $(IMPL_O) $(OBJS) $(TEST)/HammingTree.test.o
	$(CC) $(CFLAGS) $(IMPL_O) $(OBJS) $(TEST)/HammingTree.test.o -o test_HammingTree
	
test_HashTable: clean $(IMPL_O) $(OBJS) $(TEST)/HashTable.test.o
	$(CC) $(CFLAGS) $(IMPL_O) $(OBJS) $(TEST)/HashTable.test.o -o test_HashTable

test_Query: clean $(IMPL_O) $(OBJS) $(TEST)/Query.test.o
	$(CC) $(CFLAGS) $(IMPL_O) $(OBJS) $(TEST)/Query.test.o -o test_Query

test_Document: clean $(IMPL_O) $(OBJS) $(TEST)/Document.test.o
	$(CC) $(CFLAGS) $(IMPL_O) $(OBJS) $(TEST)/Document.test.o -o test_Document

run: all
	./test_Entry
	./test_LinkedList
	./test_BKTree
	./test_HammingTree
	./test_Query
	./test_Document
	./test_HashTable
	./testdriver

valgrind: all
	valgrind $(VALFLAGS) ./test_Entry
	valgrind $(VALFLAGS) ./test_LinkedList
	valgrind $(VALFLAGS) ./test_BKTree
	valgrind $(VALFLAGS) ./test_Query
	valgrind $(VALFLAGS) ./test_Document
	valgrind $(VALFLAGS) ./test_HammingTree
	valgrind $(VALFLAGS) ./test_HashTable
	valgrind $(VALFLAGS) ./testdriver


lib: $(IMPL_O) $(OBJS)
	$(CC) $(CXXFLAGS) -shared -o lib$(LIBRARY).so $(IMPL_O) $(OBJS)

testdriver: lib $(TEST_O)
	$(CXX) $(CXXFLAGS) -o testdriver $(TEST_O) ./lib$(LIBRARY).so

valgrindmain: mainonly
	valgrind $(VALFLAGS) ./main

mainonly: clean $(IMPL_O) $(OBJS) $(MAIN_O)
	$(CC) $(CFLAGS) $(IMPL_O) $(OBJS) $(MAIN_O) -o main
	mkdir -p $(ODIR)
	mv $(OBJS) $(IMPL_O) $(MAIN_O) $(ODIR)

# Delete executable & object files
clean:
	rm -f testdriver lib$(LIBRARY).so result.txt test_Entry test_LinkedList test_BKTree test_Document test_HammingTree test_Query test_HashTable main
	find . -name '*.o' -print | xargs rm -f
	rm -rf $(ODIR)
