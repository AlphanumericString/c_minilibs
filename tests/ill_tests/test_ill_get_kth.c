#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_get_kth(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	INIT_NODE(n1, 42, 1);
	INIT_NODE(n2, 43, 2);
	INIT_NODE(n3, 44, 3);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	assert(ill_get_kth(head, 0) == &n1);
	assert(ill_get_kth(head, 1) == &n2);
	assert(ill_get_kth(head, 3) == NULL);
	assert(ill_get_kth(NULL, 0) == NULL);
}
