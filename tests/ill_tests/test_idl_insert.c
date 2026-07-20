#include <assert.h>

#include "ill_tests.h"

void test_idl_insert(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n4.dnode, offsetof(t_test_dnode, dnode));

	n1.value = 1;
	n2.value = 2;
	n3.value = 3;
	n4.value = 4;

	idl_push(&head, &n2.dnode);
	assert(head == &n2.dnode);

	idl_push(&head, &n1.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n2.dnode.prev == &n1.dnode);

	idl_push_back(&head, &n4.dnode);
	assert(n2.dnode.next == &n4.dnode);
	assert(n4.dnode.prev == &n2.dnode);
	assert(n4.dnode.next == NULL);

	idl_append(&n2.dnode, &n3.dnode);
	assert(n2.dnode.next == &n3.dnode);
	assert(n3.dnode.prev == &n2.dnode);
	assert(n3.dnode.next == &n4.dnode);
	assert(n4.dnode.prev == &n3.dnode);

	t_test_dnode n5, n6;
	idl_init(&n5.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n6.dnode, offsetof(t_test_dnode, dnode));
	n5.dnode.next = &n6.dnode;
	n6.dnode.prev = &n5.dnode;

	idl_nappend(&n4.dnode, &n5.dnode);
	assert(n4.dnode.next == &n5.dnode);
	assert(n5.dnode.prev == &n4.dnode);
	assert(n5.dnode.next == &n6.dnode);
	assert(n6.dnode.next == NULL);

	t_test_dnode n7, n8;
	idl_init(&n7.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n8.dnode, offsetof(t_test_dnode, dnode));
	n7.dnode.next = &n8.dnode;
	n8.dnode.prev = &n7.dnode;

	idl_npush(&head, &n7.dnode);
	assert(head == &n7.dnode);
	assert(n8.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n8.dnode);

	t_test_dnode n0;
	idl_init(&n0.dnode, offsetof(t_test_dnode, dnode));
	idl_prepend(&n1.dnode, &n0.dnode);
	assert(n8.dnode.next == &n0.dnode);
	assert(n0.dnode.prev == &n8.dnode);
	assert(n0.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n0.dnode);
}
