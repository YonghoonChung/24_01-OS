PROGRAM = final1 final2 final3
SRCS = Console.c
OBJS = ${SRCS:.c=.o}

CC = gcc

CFLAGS = -O2 -pthread -Wno-unused-result
#DFLAGS = -g
DFLAGS = 

all: $(PROGRAM)


final1: final1.o $(OBJS)
	$(CC) $< $(OBJS) $(CFLAGS) $(DFLAGS) -o $@

final2: final2.o $(OBJS)
	$(CC) $< $(OBJS) $(CFLAGS) $(DFLAGS) -o $@

final3: final3.o $(OBJS)
	$(CC) $< $(OBJS) $(CFLAGS) $(DFLAGS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $(DFLAGS) -c $<

clean:
	rm -f $(PROGRAM) *.o

test:
	echo SRCS = $(SRCS)
	echo OBJS = $(OBJS)

