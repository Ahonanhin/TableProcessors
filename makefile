CC=gcc
CFLAGS=-Wall -Wextra -Werror -D_DEFAULT_SOURCE -std=c99

SRCS=table_processors.c 
#table_makers.c structs_def.c
OBJS=$(SRCS:.c=.o)

all: table_processors

table_processors: $(OBJS)
	$(CC) $(CFLAGS) -o table_processors $(OBJS)

table_processors.o: table_processors.c
	$(CC) $(CFLAGS) -c table_processors.c

.PHONY: help
help:
	@echo "Usage: make [TARGET]"
	@echo ""
	@echo "Targets:"
	@echo "  all            Build the program"
	@echo "  clean          Remove object files and binary"
	@echo "  help           Display this help message"

clean:
	rm -f $(OBJS) table_processors
