#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_iter(void) {
	t_cm_test_node  n1, n2, n3;
	t_cm_ill       *head  = NULL;
	t_cm_test_node *typed = NULL;
	void           *it    = NULL;

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);

	assert(ill_iter(head, &it) == false);
	assert(it == NULL);
	assert(ill_iter(NULL, &it) == false);
	assert(ill_iter(NULL, NULL) == false);

	ill_push_back_node(&head, &n1.node);
	ill_push_back_node(&head, &n2.node);
	ill_push_back_node(&head, &n3.node);

	assert(ill_iter(head, NULL) == false);

	it = NULL;
	assert(ill_iter(head, &it) == true);
	assert(it == &n1);
	assert(ill_iter(head, &it) == true);
	assert(it == &n2);
	assert(ill_iter(head, &it) == true);
	assert(it == &n3);
	assert(ill_iter(head, &it) == false);
	assert(it == NULL);

	// exhausted handles restart from the head on the next call
	assert(ill_iter(head, &it) == true);
	assert(it == &n1);

	// typed iterator usage
	assert(ill_iter(head, (void **)&typed) == true);
	assert(typed == &n1);
	assert(ill_iter(head, (void **)&typed) == true);
	assert(typed == &n2);

	// start from an arbitrary element
	it = &n2;
	assert(ill_iter(head, &it) == true);
	assert(it == &n3);
	assert(ill_iter(head, &it) == false);
	assert(it == NULL);
}
