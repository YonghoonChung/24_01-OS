#PROGRAM = hw4_2 hw4_2_ST hw4_3 hw4_3_ST
PROGRAM = hw4_2_ST hw4_3_ST
SRCS = Console.c
OBJS = ${SRCS:.c=.o}

CC = gcc

CFLAGS = -O2
DFLAGS = -g

all: $(PROGRAM)

hw4_2: hw4_2.o $(OBJS)
	$(CC) $< $(OBJS) -o $@

hw4_2_ST: hw4_2_ST.o $(OBJS)
	$(CC) $< $(OBJS) -o $@

hw4_3: hw4_3.o $(OBJS)
	$(CC) $< $(OBJS) -o $@

hw4_3_ST: hw4_3_ST.o $(OBJS)
	$(CC) $< $(OBJS) -o $@

%.o: %.c %.h
	$(CC) $(CFLAGS) $(DFLAGS) -c $<

clean:
	rm -f $(PROGRAM) *.o

test:
	echo SRCS = $(SRCS)
	echo OBJS = $(OBJS)

