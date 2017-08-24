CC=gcc
CFLAGS=-I.
DEPS = resistors.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: main.o 
	gcc -o resistor_finder $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o resistor_finder
