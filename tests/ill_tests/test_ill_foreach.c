#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_foreach(void) {
	t_test_node n1, n2, n3;
	t_ill      *head   = NULL;
	int         to_add = 5;

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	assert(ill_foreach(head, cb_ill_add_val_as_int_ptr, &to_add) == true);
	assert(n1.value == 10 + to_add);
	assert(n2.value == 20 + to_add);
	assert(n3.value == 30 + to_add);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	assert(ill_foreach(head, cb_ill_stop_at_30, NULL) == false);
	assert(ill_foreach(head, NULL, &to_add) == false);
	assert(ill_foreach(NULL, cb_ill_add_val_as_int_ptr, &to_add) == false);
}
