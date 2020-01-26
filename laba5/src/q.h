#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>
#include <stdint.h>

#define POOL_SIZE 100
#define QueueElemType int32_t

typedef struct
{
	int first;
	size_t size;
	int32_t data[POOL_SIZE];
}queue;


void qCreate(queue* q);
bool qEmpty(const queue* q);
int qSize(queue* q);
int qTop(queue* q);
int qPush(queue* q, int t);
int qPop(queue* q);
void printq(queue* q);



#endif