#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_get_min(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	assert(ill_get_min(head, cb_ill_cmp_min_max) == &n1);
	n1.value = 30;
	n2.value = 20;
	n3.value = 10;
	assert(ill_get_min(head, cb_ill_cmp_min_max) == &n3);

	assert(ill_get_min(NULL, cb_ill_cmp_min_max) == NULL);
	assert(ill_get_min(head, NULL) == NULL);
}
