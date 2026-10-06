#include <assert.h>

#include "ill_tests.h"

void test_idl_find(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;
	t_idl       *not_found, *found;
	t_idl       *prev, *prev_head, *prev_not_found;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);

	found = idl_find(head, &n2.dnode);
	assert(found == &n2.dnode);

	not_found = idl_find(head, &n3.dnode);
	assert(not_found == NULL);
	not_found = idl_find(NULL, &n3.dnode);
	assert(not_found == NULL);
	not_found = idl_find(head, NULL);
	assert(not_found == NULL);

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
