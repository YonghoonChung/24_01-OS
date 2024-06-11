#define MAX_LEN 32
#define QUEUE_SIZE 10

typedef struct {
	int in, out;
	int count;
	char buffer[QUEUE_SIZE][MAX_LEN];
} StringQueue;

void StringQueue_Init(StringQueue *q);
void StringQueue_Insert(StringQueue *q, char *str);
void StringQueue_Delete(StringQueue *q, char *strbuf);
void StringQueue_Display(StringQueue *q, int sx, int sy);
