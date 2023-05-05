#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
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
  int length;

};

queue_t queue_create(void)
{
	/* TODO Phase 1 */
  
  queue_t newQueue = (queue_t) malloc(sizeof(struct queue));
  newQueue->first = NULL;
  newQueue->last = NULL;
  return newQueue;

}

int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->first != NULL) {
		return -1;
	}
	else {
		free(queue);
	}
    return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */

	// case 1: queue has 0 elements
	if (queue->first == NULL) {
		queue->first = (struct node*) malloc(sizeof(struct node));
		queue->first->data = data;
		queue->last = queue->first;
        queue->length = 0;
        bool exists = queue->first->data != NULL;
        printf("%d\n",exists);
        printf("A\n");
        
	}
	
	else if (queue->first == queue->last) {
		queue->last = (struct node*) malloc(sizeof(struct node));
		queue->last->data = data; // update last to point to new node
		queue->last->next = NULL; 
		queue->first->next = queue->last; // update first so its next points to last
		queue->last->prev = queue->first; // update last so its prev points to first
        queue->length++;
        bool exists = queue->first->data != NULL;
        printf("%d\n",exists);
        printf("B\n");
	}

	else {
		queue->last->next = (struct node*) malloc(sizeof(struct node));
		queue->last->next->data = data;
		queue->last->next->prev = queue->last;
		queue->last = queue->last->next;
        queue->length++;
        bool exists = queue->first->data != NULL;
        printf("%d\n",exists);
        printf("C\n");
	}
    
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if (queue == NULL || queue->last == NULL || queue->last->data == NULL) {
		return -1;
	}
    
	*data = queue->last->data;
    // case 1: 1 element in queue
    if (queue->first == queue->last) {
        queue->length--;
	    free(queue->last);
        queue->last = NULL;
    }
    else {
        queue->last = queue->last->prev;
        queue->length--;
        free(queue->last->next);
    }
	return 0;

}

int queue_delete(queue_t queue, void *data) {
    if (queue == NULL || data == NULL) {
        return -1;
    }

    struct node *curr = queue->first;
    while (curr != NULL) {
        if (curr->data == data) {
            // Remove node from the list
            if (curr->prev != NULL) {
                curr->prev->next = curr->next;
            } else {
                queue->first = curr->next;
            }

            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            } else {
                queue->last = curr->prev;
            }

            free(curr);
            queue->length--;
            return 0;
        }

        curr = curr->next;
    }

    return -1; // Data not found in queue
}


int queue_iterate(queue_t queue, queue_func_t func)
{
	if(queue == NULL || func == NULL)
        return -1;
    
    struct node *curr = queue->first;
    while(curr != NULL){
        func(queue,curr->data);
        curr = curr->next;
    }
    return 0;
}

int queue_length(queue_t queue)
{
	if(queue == NULL)
        return -1;
    return queue->length;
}
