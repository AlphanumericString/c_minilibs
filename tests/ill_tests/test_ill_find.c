#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_find(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	INIT_NODE(n1, 42, 1);
	INIT_NODE(n2, 43, 2);
	INIT_NODE(n3, 44, 3);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);

	assert(ill_find(head, &n2.node) == &n2.node);
	assert(ill_find(head, &n3.node) == NULL);
	assert(ill_find(head, NULL) == NULL);
	assert(ill_find(NULL, &n2.node) == NULL);
}
