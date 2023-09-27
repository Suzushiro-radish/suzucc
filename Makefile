CFLAGS=-std=c11 -g -static

suzucc: suzucc.c

test: suzucc
	./test.sh

clean:
	rm -f suzucc *.o *~ tmp*

.PHONY: test clean