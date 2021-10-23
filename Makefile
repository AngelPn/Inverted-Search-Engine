#paths
INCL = include
SRC = src
MODULES = mylib
ODIR = build

#compiler
CC = gcc

#executable's name
EXEC = searchEngine

#compile options
CFLAGS = -Wall -g -I$(MODULES)/EntryList -I$(MODULES)/Entry -I$(MODULES)/BKTree -I$(INCL)

OBJS =  $(SRC)/main.o
OBJS += $(MODULES)/Entry/Entry.o
OBJS += $(MODULES)/EntryList/EntryList.o
OBJS += $(MODULES)/BKTree/BKTree.o

$(EXEC): clean $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) -lm
	mkdir -p $(ODIR)
	mv $(OBJS) $(ODIR)

# delete executable & object files
clean:
	rm -f $(EXEC)
	rm -rf $(ODIR)