IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=.

LIBS=

_DEPS = atf.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = atf.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

atf: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

compile_test_generation: $(OBJ) test_generation.o
	$(CC) -o test_generation $^ $(CFLAGS) $(LIBS)

test_generation: compile_test_generation
	./test_generation

compile_test_storage: $(OBJ) test_storage.o
	$(CC) -o test_storage $^ $(CFLAGS) $(LIBS)

test_storage: compile_test_storage
	./test_storage

compile_test_exploration: $(OBJ) test_exploration.o
	$(CC) -o test_exploration $^ $(CFLAGS) $(LIBS)

test_exploration: compile_test_exploration
	./test_exploration

compile_test_final: $(OBJ) test_final.o
	$(CC) -o test_final $^ $(CFLAGS) $(LIBS)

test_final: compile_test_final
	./test_final

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ test_generation test_storage test_exploration test_final
