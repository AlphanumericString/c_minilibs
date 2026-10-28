#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_nlength(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	INIT_STD_NODES(n1, n2, n3, n4);

	assert(ill_nlength(head, 10) == 0);
	assert(ill_nlength(NULL, 10) == 0);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);
	ill_push_back(&head, &n4.node);

	assert(ill_nlength(head, 2) == 2);
	assert(ill_nlength(head, 10) == 4);
}
