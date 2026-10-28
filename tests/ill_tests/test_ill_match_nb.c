#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_match_nb(void) {
	t_test_node n1, n2, n3, n4;
	t_ill      *head = NULL;

	INIT_NODE(n1, 1, 1);
	INIT_NODE(n2, 2, 2);
	INIT_NODE(n3, 3, 3);
	INIT_NODE(n4, 4, 4);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);
	ill_push_back(&head, &n4.node);

	assert(ill_match_nb(head, cb_ill_is_even, NULL) == 2);
	assert(ill_match_nb(head, NULL, NULL) == 0);
	assert(ill_match_nb(NULL, cb_ill_is_even, NULL) == 0);
}
