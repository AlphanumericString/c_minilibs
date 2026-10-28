#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_find_prev(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;
	t_idl       *prev, *prev_head, *prev_not_found;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);

	prev = idl_find_prev(head, &n2.dnode);
	assert(prev == &n1.dnode);

	prev_head = idl_find_prev(head, &n1.dnode);
	assert(prev_head == NULL);
	prev_not_found = idl_find_prev(head, &n3.dnode);
	assert(prev_not_found == NULL);
	prev_not_found = idl_find_prev(NULL, &n3.dnode);
	assert(prev_not_found == NULL);
	prev_not_found = idl_find_prev(head, NULL);
	assert(prev_not_found == NULL);
}
