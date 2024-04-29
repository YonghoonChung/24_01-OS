#PROGRAM = hw4_2 hw4_2_ST hw4_3 hw4_3_ST
#PROGRAM = hw4_2_ST hw4_3_ST
PROGRAM = midterm1_21900670 midterm2_21900670 
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

%.o: %.c %.h
	$(CC) $(CFLAGS) $(DFLAGS) -c $<

clean:
	rm -f $(PROGRAM) *.o

test:
	echo SRCS = $(SRCS)
	echo OBJS = $(OBJS)

