#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_get(void) {
	t_test_node n1, n2, n3;
	t_ill      *head  = NULL;
	int         val20 = 20;
	int         val40 = 40;

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	assert(ill_get(head, cb_ill_cmp_val, &val20) == &n2.node);
	assert(ill_get(head, cb_ill_cmp_val, &val40) == NULL);
	assert(ill_get(head, NULL, &val40) == NULL);
	assert(ill_get(NULL, cb_ill_cmp_val, &val40) == NULL);
}
