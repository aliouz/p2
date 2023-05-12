#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");
	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");
	//ptr = &data;
	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);

}

// checks if the destory function works for all cases
void test_destroy(void)
{
	fprintf(stderr, "*** TEST delete with an unitialized queue ***\n");
	queue_t q = NULL;
	TEST_ASSERT(queue_destroy(q) == -1);

	fprintf(stderr, "*** TEST delete with an unempty queue ***\n");

	q = queue_create();
	int data = 3, *ptr;
	queue_enqueue(q, &data);
	TEST_ASSERT(queue_destroy(q) == -1);

	fprintf(stderr, "*** TEST delete with an empty queue ***\n");

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_destroy(q) == 0);
}

// deletes element from queue if the element is a specific value 
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

// checks if the interator function works
void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    assert(data[0] == 2);
    assert(queue_length(q) == 9);
}

// checks that dequeue works for all cases
void test_dequeue(){
	queue_t queue;
	queue = queue_create();

	int data = 3, *ptr;
	ptr = &data;
	TEST_ASSERT(queue_dequeue(queue,(void**)&ptr) == -1);

}

int main(void)
{
	test_create();
	test_queue_simple();
	test_destroy();
	test_iterator();
	test_dequeue();
	return 0;
}
