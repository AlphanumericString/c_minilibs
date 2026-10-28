#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_remove_node(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head      = NULL;
	t_idl       *null_head = NULL;
	bool         rm_node;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	idl_push_back(&head, &n2.dnode);
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
