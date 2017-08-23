CC=gcc
CFLAGS=-I.
DEPS = resistors.h output_log.PHONY


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

resistor_finder: main.o 
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o resistor_finder
