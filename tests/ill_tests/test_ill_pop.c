#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_pop_node(void) {
	t_cm_test_node n1, n2, n3, n4;
	t_cm_ill      *head = NULL;

	INIT_STD_NODES(n1, n2, n3, n4);

	assert(ill_pop_node(NULL) == NULL);
	assert(ill_pop_node(&head) == NULL);

	ill_push_back_node(&head, &n1.node);
	ill_push_back_node(&head, &n2.node);
	ill_push_back_node(&head, &n3.node);
	ill_push_back_node(&head, &n4.node);

	assert(ill_pop_node(&head) == &n1.node);
	assert(head == &n2.node);
}
