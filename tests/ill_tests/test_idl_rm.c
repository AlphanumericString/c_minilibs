#include <assert.h>

#include "ill_tests.h"

void test_idl_rm(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;
	t_idl       *pop_res;
	t_idl       *null_head = NULL;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n4.dnode, offsetof(t_test_dnode, dnode));

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	assert(n1.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n1.dnode, &n2.dnode);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);

	pop_res = idl_pop(&head);
	assert(pop_res == &n1.dnode);

	assert(head == &n2.dnode);
	assert(n2.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);

	assert(idl_pop(&null_head) == NULL);
	assert(idl_pop(NULL) == NULL);
	// TODO: test were pop until !head

	pop_res = idl_pop_back(&head);
	assert(pop_res == &n4.dnode);
	assert(head == &n2.dnode);
	assert(n2.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	assert(n3.dnode.next == NULL);

	assert(idl_pop_back(&null_head) == NULL);
	assert(idl_pop_back(NULL) == NULL);

	idl_push_back(&head, &n4.dnode);
	assert(n2.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);
	// test with bad tail
	n4.dnode.prev = NULL;
	assert(idl_pop_back(&head) != NULL);
	assert(idl_pop_back(&head) == &n4.dnode);
	assert(idl_pop_back(&head) == &n4.dnode);
	assert(idl_pop_back(&head) == &n4.dnode);
	n4.dnode.prev = &n3.dnode;
	idl_pop_back(&head);
	// TODO: test were pop_back until !head

	t_idl *rm_after_res;
	rm_after_res = idl_remove_after(&n2.dnode);
	assert(rm_after_res == &n3.dnode);
	assert(n2.dnode.next == NULL);
	assert(head == &n2.dnode);
	assert(idl_remove_after(idl_get_tail(&n2.dnode)) == NULL);
	assert(idl_remove_after(NULL) == NULL);

	bool rm_node;
	idl_push_back(&head, &n3.dnode);
	rm_node = idl_remove_node(&head, &n3.dnode); // rm last
	assert(rm_node == true);
	assert(head == &n2.dnode);
	assert(n2.dnode.next == NULL);

	assert(idl_remove_node(NULL, &n3.dnode) == false);
	assert(idl_remove_node(&head, NULL) == false);
	assert(idl_remove_node(&null_head, &n3.dnode) == false);
	assert(idl_remove_node(&head, (void *)0xDEAD) == false);

	rm_node = idl_remove_node(&head, &n2.dnode);
	assert(rm_node == true);
	assert(head == NULL);
	head = &n1.dnode;
	idl_prepend(&n2.dnode, &n1.dnode);
	rm_node = idl_remove_node(&head, &n1.dnode);
	assert(head == &n2.dnode && head->next == NULL);
}
