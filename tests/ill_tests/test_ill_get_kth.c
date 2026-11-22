#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_get_kth_node(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	INIT_NODE(n1, 42, 1);
	INIT_NODE(n2, 43, 2);
	INIT_NODE(n3, 44, 3);

	ill_push_back_node(&head, &n1.node);
	ill_push_back_node(&head, &n2.node);
	ill_push_back_node(&head, &n3.node);

	assert(ill_get_kth_node(head, 0) == &n1.node);
	assert(ill_get_kth_node(head, 1) == &n2.node);
	assert(ill_get_kth_node(head, 3) == NULL);
	assert(ill_get_kth_node(NULL, 0) == NULL);
}
