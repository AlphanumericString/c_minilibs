#include <assert.h>

#include "ill_tests.h"

NO_COV static void remove_on_many_elem(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;
	bool         rm_node;

	INIT_STD_DNODES(n1, n2, n3, n4);
	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);
	assert(n1.dnode.prev == NULL); // state start
	ASSERT_ORDER_IDL(&n1.dnode, &n2.dnode);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);

	rm_node = idl_remove_node(&head, &n4.dnode); // rm last
	assert(rm_node == true);
	assert(head == &n1.dnode); // head unchanged
	assert(n1.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n1.dnode, &n2.dnode);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	assert(n3.dnode.next == NULL);

	head = NULL; // state reset
	INIT_STD_DNODES(n1, n2, n3, n4);
	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);
	rm_node = idl_remove_node(&head, &n1.dnode); // rm 1st
	assert(rm_node == true);
	assert(head == &n2.dnode); // head changed
	assert(n2.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n2.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);

	head = NULL; // state reset
	INIT_STD_DNODES(n1, n2, n3, n4);
	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);
	rm_node = idl_remove_node(&head, &n2.dnode); // rm middle
	assert(rm_node == true);
	assert(head == &n1.dnode);
	assert(n1.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n1.dnode, &n3.dnode);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);

	head = NULL; // state reset
	INIT_STD_DNODES(n1, n2, n3, n4);
	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);
	n2.dnode.prev = NULL;
	// rm middle with malformed node
	rm_node       = idl_remove_node(&head, &n2.dnode);
	assert(rm_node == true);
	assert(head == &n1.dnode);
	// feed broken = get back broken
	assert(n1.dnode.prev == NULL);
	assert(n1.dnode.next == &n2.dnode && n3.dnode.prev == NULL);
	ASSERT_ORDER_IDL(&n3.dnode, &n4.dnode);
	assert(n4.dnode.next == NULL);
}

NO_COV static void remove_on_2_elem(void) {
	t_test_dnode n1, n2;
	t_idl       *head = NULL;
	bool         rm_node;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);

	head = NULL;
	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);

	rm_node = idl_remove_node(&head, &n1.dnode);
	assert(rm_node == true);
	assert(head == &n2.dnode && head->next == NULL);

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	head = NULL;
	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);

	rm_node = idl_remove_node(&head, &n2.dnode);
	assert(rm_node == true);
	assert(head == &n1.dnode && head->next == NULL);
}

NO_COV static void remove_on_1_elem(void) {
	t_test_dnode n1;
	t_idl       *head = NULL;
	bool         rm_node;

	INIT_DNODE(n1, 42, 1);

	head = NULL;
	idl_push_back(&head, &n1.dnode);

	rm_node = idl_remove_node(&head, &n1.dnode);
	assert(rm_node == true);
	assert(head == NULL);
}

NO_COV void test_idl_remove_node(void) {
	t_test_dnode n1;
	t_idl       *head           = NULL;
	t_idl       *null_head      = NULL;
	void        *random_address = (void *)0xDEAD;

	remove_on_2_elem();
	remove_on_many_elem();
	remove_on_1_elem();

	INIT_DNODE(n1, 42, 1);
	idl_push_back(&head, &n1.dnode);
	assert(idl_remove_node(NULL, random_address) == false);
	assert(idl_remove_node(&head, NULL) == false);
	assert(idl_remove_node(&null_head, random_address) == false);
	assert(idl_remove_node(&head, random_address) == false);
}
