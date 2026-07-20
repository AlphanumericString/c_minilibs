#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

static void *duplicate_dnode(const void *elem) {
	t_test_dnode *src  = (t_test_dnode *)elem;
	t_test_dnode *copy = malloc(sizeof(t_test_dnode));
	if (copy) {
		copy->id    = src->id;
		copy->value = src->value;
		idl_init(&copy->dnode, offsetof(t_test_dnode, dnode));
	}
	return copy;
}

void test_idl_init(void) {
	t_test_dnode node;

	idl_init(&node.dnode, offsetof(t_test_dnode, dnode));

	assert(node.dnode.offset == offsetof(t_test_dnode, dnode));
	assert(node.dnode.next == NULL);
	assert(node.dnode.prev == NULL);

	t_test_dnode node2;
	idl_init(&node2.dnode, offsetof(t_test_dnode, dnode));
	node.id          = 1;
	node.value       = 42;
	node.dnode.next  = &node2.dnode;
	node2.id         = 2;
	node2.value      = 84;
	node2.dnode.prev = &node.dnode;

	t_test_dnode *copy = idl_copy(&node.dnode, duplicate_dnode);

	assert(copy != NULL);
	assert(copy->id == 1);
	assert(copy->value == 42);
	assert(copy->dnode.next != NULL);
	assert(copy->dnode.prev == NULL);

	t_test_dnode *copy_next = (t_test_dnode *)idl_ntoe(copy->dnode.next);
	assert(copy_next->id == 2);
	assert(copy_next->value == 84);
	assert(copy_next->dnode.next == NULL);
	assert(copy_next->dnode.prev == &copy->dnode);

	free(copy_next);
	free(copy);
}
