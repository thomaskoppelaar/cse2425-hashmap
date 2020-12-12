# Usage:
# Run 'make test' to execute the test program.
# Run 'make valgrind' to run the test program in Valgrind.
# Run 'make clean' to remove compiled files.

CC = gcc
CPPFLAGS = -g -Wall -Werror -Wextra -Wno-unused-parameter -Wno-unused-variable -pedantic
LDFLAGS = -lpthread -lrt
OBJS = test.c gest.c
TARGET = test

.PHONY: test valgrind submit clean

default: test

compile : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(CPPFLAGS) -o $(TARGET)

test: compile
	./test

valgrind:
	valgrind --tool=memcheck --track-origins=yes --show-leak-kinds=all ./test

clean:
	-rm -f *.o
	-rm -f $(TARGET)
