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
OBJS = $(MODULES)/Entry/Entry.o $(MODULES)/LinkedList/LinkedList.o $(MODULES)/BKTree/BKTree.o 
OBJS += $(MODULES)/HashTable/HashTable.o $(MODULES)/HammingTree/HammingTree.o

# Compilers
CC  = gcc
CXX = g++

# Compile flags
CFLAGS = -O3 -fPIC -mavx2 -Wall -g -I$(INCL) -I$(MODULES)/Entry -I$(MODULES)/LinkedList -I$(MODULES)/Entry -I$(MODULES)/BKTree -I$(MODULES)/HammingTree
CXXFLAGS = $(CFLAGS)
LDFLAGS = -lpthread
# Valgrind flags
VALFLAGS = --leak-check=full --track-origins=yes -s

PROGRAMS = test_Entry test_LinkedList test_BKTree testdriver 
LIBRARY = core

all: $(PROGRAMS)
	mkdir -p $(ODIR)
	mv $(OBJS) $(IMPL_O) $(TEST_O) $(TEST)/Entry.test.o $(TEST)/LinkedList.test.o $(TEST)/BKTree.test.o $(ODIR)

mainonly: clean $(OBJS) $(MODULES)/HashTable/HashTable.o $(MODULES)/HammingTree/HammingTree.o $(MODULES)/main.o
	$(CC) $(CFLAGS) $(OBJS) $(MODULES)/main.o -o main
	mkdir -p $(ODIR)
	mv $(OBJS) $(MODULES)/main.o $(ODIR)

test_Entry: clean $(OBJS) $(TEST)/Entry.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/Entry.test.o -o test_Entry

test_LinkedList: clean $(OBJS) $(TEST)/LinkedList.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/LinkedList.test.o -o test_LinkedList

test_BKTree: clean $(OBJS) $(TEST)/BKTree.test.o
	$(CC) $(CFLAGS) $(OBJS) $(TEST)/BKTree.test.o -o test_BKTree

run: all
	./test_Entry
	./test_LinkedList
	./test_BKTree

valgrind: all
	valgrind $(VALFLAGS) ./test_Entry
	valgrind $(VALFLAGS) ./test_LinkedList
	valgrind $(VALFLAGS) ./test_BKTree

lib: $(IMPL_O) $(OBJS)
	$(CC) $(CXXFLAGS) -shared -o lib$(LIBRARY).so $(IMPL_O) $(OBJS)

testdriver: lib $(TEST_O)
	$(CXX) $(CXXFLAGS) -o testdriver $(TEST_O) ./lib$(LIBRARY).so

# Delete executable & object files
clean:
	rm -f testdriver lib$(LIBRARY).so result.txt test_Entry test_LinkedList test_BKTree main
	find . -name '*.o' -print | xargs rm -f
	rm -rf $(ODIR)
