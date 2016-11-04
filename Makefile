all: main

OBJS=main.o misc.o parser.o scanner.o stack.o symbol_table.o

OUTDIR=build
SRCDIR=src
INCLDIR=include

CC=gcc

main: $(patsubst %, $(OUTDIR)/%, $(OBJS))
	$(CC) -o $@ $^

$(OUTDIR)/main.o: $(SRCDIR)/main.c $(INCLDIR)/scanner.h
	$(CC) -c  $< -o $@

$(OUTDIR)/misc.o: $(SRCDIR)/misc.c $(INCLDIR)/misc.h $(INCLDIR)/types.h
	$(CC) -c $< -o $@

$(OUTDIR)/parser.o: $(SRCDIR)/parser.c $(patsubst %, $(INCLDIR)/%, parser.h scanner.h stack.h types.h symbol_table.h)
	$(CC) -c $< -o $@

$(OUTDIR)/scanner.o: $(SRCDIR)/scanner.c $(patsubst %, $(INCLDIR)/%, global_consts.h misc.h types.h parser.h symbol_table.h)
	$(CC) -c $< -o $@

$(OUTDIR)/stack.o: $(SRCDIR)/stack.c $(INCLDIR)/stack.h $(INCLDIR)/global_consts.h
	$(CC) -c $< -o $@

$(OUTDIR)/symbol_table.o: $(SRCDIR)/symbol_table.c $(INCLDIR)/symbol_table.h $(INCLDIR)/global_consts.h
	$(CC) -c $< -o $@

.PHONY:
	clean

clean:
	rm build/*.o
