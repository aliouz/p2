#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"



struct node {
   void* data;
   struct node *next;
   struct node *prev;
};


struct queue {
	/* TODO Phase 1 */
  struct node *first;
  struct node *last;

};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
  
  queue_t newQueue = (queue_t) malloc(sizeof(struct queue));
  newQueue->first = NULL;
  newQueue->last = NULL;
  return &newQueue;

}

int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->first != NULL) {
		return -1;
	}
	else {
		free(queue);
	}
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */

	// case 1: queue has 0 elements
	if (queue->first == NULL) {
		queue->first = (struct node*) malloc(sizeof(struct node));
		queue->first->data = data;
		queue->last = queue->first;
	}
	
	else if (queue->first == queue->last) {
		queue->last = (struct node*) malloc(sizeof(struct node));
		queue->last->data = data; // update last to point to new node
		queue->last->next = NULL; 
		queue->first->next = queue->last; // update first so its next points to last
		queue->last->prev = queue->first; // update last so its prev points to first
	}

	else {
		queue->last->next = (struct node*) malloc(sizeof(struct node));
		queue->last->next->data = data;
		queue->last->next->prev = queue->last;
		queue->last = queue->last->next;

	}
	
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if (queue == NULL || queue->last == NULL || queue->last->data == NULL) {
		return -1;
	}
	else {
		data = queue->last->data;
		queue->last = queue->last->prev;
		free(queue->last->next);
		return 0;
	}

}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}
