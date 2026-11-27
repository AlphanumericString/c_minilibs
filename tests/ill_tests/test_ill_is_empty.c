#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_is_empty(void) {
	t_cm_test_node n1;
	t_cm_ill      *head = NULL;

	INIT_NODE(n1, 10, 1);

	assert(ill_is_empty(NULL) == true);
	assert(ill_is_empty(head) == true);
	ill_push_node(&head, &n1.node);
	assert(ill_is_empty(head) == false);
}
