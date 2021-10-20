CC = gcc
CFLAGS = -std=c99 -Wall

INCLUDES = -I./include
OBJDIR = obj
SRCDIR = src

MAIN = battleship_64

_SRCS = easy_io.c main.c player.c game.c cpu.c
SRCS = $(patsubst %,$(SRCDIR)/%,$(_SRCS))

# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
_OBJS = $(_SRCS:.c=.o)
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

all:	$(MAIN)
		@echo Created executable file $(MAIN)

$(MAIN): 	$(OBJS)
			$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS)

$(OBJDIR)/%.o:	$(SRCDIR)/%.c
			$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
