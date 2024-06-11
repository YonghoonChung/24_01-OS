#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Console.h"
#include "StringQueue.h"

void StringQueue_Init(StringQueue *q)
{
	q->in = q->out = q->count = 0;

	for(int i = 0; i < QUEUE_SIZE; i++)
		q->buffer[i][0] = q->buffer[i][MAX_LEN - 1] = 0;
}

void StringQueue_Insert(StringQueue *q, char *str)
{
	if(q->count == QUEUE_SIZE){
		printf("Queue is full!\n");
		return;
	}

	strncpy(q->buffer[q->in], str, 31);

	q->in = (q->in + 1) % QUEUE_SIZE;
	q->count++;
}

void StringQueue_Delete(StringQueue *q, char *strbuf)
{
	if(q->count == 0){
		printf("Queue is empty!\n");
		return;
	}

	strcpy(strbuf, q->buffer[q->out]);
	q->buffer[q->out][0] = 0;

	q->out = (q->out + 1) % QUEUE_SIZE;
	q->count--;
}

void StringQueue_Display(StringQueue *q, int sx, int sy)
{
	for(int i = 0; i < QUEUE_SIZE; i++){
		PrintXY(sx, sy + i, " ");
		PrintXY(sx + 35, sy + i, " ");

		if(q->buffer[i][0])
			PrintXY(sx + 1, sy + i, "[%-32s]", q->buffer[i]);
		else
			PrintXY(sx + 1, sy + i, "[%-32s]", " ");
	}

	PrintXY(sx, sy + q->in, ">");
	PrintXY(sx + 35, sy + q->out, ">");

	fflush(stdout);
}

