#PROGRAM = midterm1_21900670 midterm2_21900670 
PROGRAM = midterm1_21900670 midterm2_21900670 midterm3_21900670
SRCS = Console.c
OBJS = ${SRCS:.c=.o}

CC = gcc

CFLAGS = -O2
DFLAGS = -g

all: $(PROGRAM)

midterm1_21900670: midterm1_21900670.o $(OBJS)
	$(CC) $< $(OBJS) -o $@

midterm2_21900670: midterm2_21900670.o $(OBJS)
	$(CC) $< $(OBJS) -o $@

midterm3_21900670: midterm3_21900670.o $(OBJS)
	$(CC) $< $(OBJS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $(DFLAGS) -c $<

clean:
	rm -f $(PROGRAM) *.o

test:
	echo SRCS = $(SRCS)
	echo OBJS = $(OBJS)

