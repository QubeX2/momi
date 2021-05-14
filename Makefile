CC=gcc
CFLAGS= -Wall -Wextra -Werror -Wfloat-equal -ggdb -I/usr/local/opt/ncurses/include/
# -save-temps : debugging macros -O2
LFLAGS=-L/usr/local/opt/ncurses/lib
LIBS=-lncursesw
# LIBS= -lm
SRCS=momi.c term.c row.c edit.c input.c output.c config.c file.c buffer.c string.c
OBJS=$(SRCS:%.c=$(OUTDIR)/%.o)
SXOBJS=$(SYNTAX:%.c=$(OUTDIR)/%.o)
OUTDIR=bin
SYNTAX=sx_makefile.c
SYNTAXDIR=syntax
MAIN=momi

.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS) $(SXOBJS)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(MAIN) $(OBJS) $(SXOBJS) $(LFLAGS) $(LIBS)

$(OUTDIR)/%.o: %.c makedirs
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(SYNTAXDIR)/%.c makedirs
	$(CC) $(CFLAGS) -c $< -o $@


makedirs:
	@mkdir -p $(OUTDIR)

clean:
	$(RM) $(OUTDIR)/*.o *~ $(OUTDIR)/$(MAIN)
