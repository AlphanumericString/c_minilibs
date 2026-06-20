#include "ill_tests.h"

void test_idl_rm(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n4.dnode, offsetof(t_test_dnode, dnode));

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	t_idl *popped = idl_pop(&head);
	assert(popped == &n1.dnode);
	assert(head == &n2.dnode);
	assert(n2.dnode.prev == NULL);

	t_idl *popped_back = idl_pop_back(&head);
	assert(popped_back == &n4.dnode);
	assert(n3.dnode.next == NULL);

	t_idl *rm_after = idl_remove_after(&n2.dnode);
	assert(rm_after == &n3.dnode);
	assert(n2.dnode.next == NULL);
	assert(head == &n2.dnode);

	idl_push_back(&head, &n3.dnode);
	bool rm_node = idl_remove_node(&head, &n3.dnode);
	assert(rm_node == true);
	assert(head == &n2.dnode);
	assert(n2.dnode.next == NULL);

	rm_node = idl_remove_node(&head, &n2.dnode);
	assert(rm_node == true);
	assert(head == NULL);
}
