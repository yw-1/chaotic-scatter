CC=gcc
CFLAGS=-I.
LIBS=-lm -lSDL2
OBJ=scatter.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
scatter: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
clean:
	rm -rf *.o
.PHONY: clean
