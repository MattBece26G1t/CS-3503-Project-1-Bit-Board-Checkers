# [Makefile] file
# builds and links all program source files into one executable

# compiler used to build the project
CC = gcc

# compiler flafs used to build the project
# enable standard/compiler warnings and specify compiler to follow langauge standard  
CFLAGS = -Wall -Wextra -std=c11

# list of object files generated from source files (.c)
# each .o file corresponds to its .c source counterpart
OBJS = main.o bitoperations.o game.o consoleUI.o saveload.o
# name of the final executable program
TARGET = bitboardcheckers

# default build target, compiles everything and produces the final program
all: $(TARGET) 

# combines all object files into one executable output
$(TARGET): $(OBJS)
$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# compile rules for each source file dependency
# ensures each object file (.o) is up to date if its .c or .h changed
main.o: main.c bitoperations.h game.h consoleUI.h saveload.h
bitoperations.o: bitoperations.c bitoperations.h
game.o: game.c game.h
consoleUI.o: consoleUI.c consoleUI.h game.h
saveload.o: saveload.c saveload.h game.h

# declare "phony" targets to specify that these are commands, not actual files (for extra caution)
.PHONY: all clean 
# use this command to perform a fresh rebuild of the entire project
# removes all generated object files (.o) and the compiled executable
clean:
	rm -f *.o $(TARGET) $(TARGET).exe