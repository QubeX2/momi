CC=gcc
CFLAGS= -Wall -Wextra -Werror -Wfloat-equal -O2 -g -I/usr/local/opt/ncurses/include/
# -save-temps : debugging macros
LFLAGS=-L/usr/local/opt/ncurses/lib
LIBS=-lncursesw
# LIBS= -lm
SRCS=momi.c term.c row.c edit.c input.c output.c config.c
OBJS=$(SRCS:%.c=$(OUTDIR)/%.o)
MAIN=momi
OUTDIR=bin

.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(OUTDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OUTDIR)/*.o *~ $(OUTDIR)/$(MAIN)