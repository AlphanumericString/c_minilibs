#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_remove_after(void) {
	t_cm_test_node n2, n3;
	t_cm_ill      *head = NULL;
	t_cm_ill      *rm_after;

	INIT_NODE(n2, 42, 2);
	INIT_NODE(n3, 43, 3);

	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	rm_after = ill_remove_after(&n2.node);
	assert(rm_after == &n3.node);
	assert(n2.node.next == NULL);
	assert(head == &n2.node);
	assert(ill_remove_after(ill_get_tail(head)) == NULL);
	assert(ill_remove_after(NULL) == NULL);
}
