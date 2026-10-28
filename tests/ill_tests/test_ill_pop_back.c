#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_pop_back(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	INIT_STD_NODES(n1, n2, n3, n4);

	assert(ill_pop_back(NULL) == NULL);
	assert(ill_pop_back(&head) == NULL);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);
	ill_push_back(&head, &n4.node);

	assert(ill_pop_back(&head) == &n4.node);
	assert(n3.node.next == NULL);
}
