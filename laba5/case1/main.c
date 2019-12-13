#include <stdio.h>
#include "q.h"



int main() {
  queue q;
  qCreate(&q);
  qPush(&q, 2);
  qPush(&q, 99);
  printf("size: %d\n", qSize(&q));
  printq(&q);
  for (int i = 0; i < 5; i++) {
    qPush(&q, i);
  }
  printq(&q);
  printf("size: %d\n", qSize(&q));
  qPop(&q);
  printf("size: %d\n", qSize(&q));
}