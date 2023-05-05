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

int main(void)
{
	test_create();
	test_queue_simple();
	test_destroy();
	return 0;
}
