CFLAGS=-std=c11 -g -fno-common
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

suzucc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): suzucc.h

test: suzucc
	./test.sh

clean:
	rm -f suzucc *.o *~ tmp*

debug: suzucc
	gdb ./suzucc

.PHONY: test clean