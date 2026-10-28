#include <assert.h>

#include "ill_tests.h"

NO_COV void test_ill_for(void) {
	t_test_node n1, n2, n3;
	t_ill      *head   = NULL;
	int         to_add = 5;

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	assert(ill_for(head, 2, cb_ill_add_val_as_int_ptr, &to_add) == true);
	assert(n1.value == 10 + to_add * 1);
	assert(n2.value == 20 + to_add * 1);
	assert(n3.value == 30 + to_add * 0);
	assert(ill_for(head, 1, cb_ill_add_val_as_int_ptr, &to_add) == true);
	assert(n1.value == 10 + to_add * 2);
	assert(n2.value == 20 + to_add * 1);
	assert(n3.value == 30 + to_add * 0);
	assert(ill_for(head, 3, cb_ill_add_val_as_int_ptr, &to_add) == true);
	assert(n1.value == 10 + to_add * 3);
	assert(n2.value == 20 + to_add * 2);
	assert(n3.value == 30 + to_add * 1);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	assert(ill_for(head, 2, cb_ill_stop_at_30, NULL) == true);
	assert(ill_for(head, 3, cb_ill_stop_at_30, NULL) == false);
	assert(ill_for(NULL, 3, cb_ill_stop_at_30, NULL) == false);
	assert(ill_for(head, 3, NULL, NULL) == false);
}
