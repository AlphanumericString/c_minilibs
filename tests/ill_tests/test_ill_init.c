#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

static void *duplicate_node(const void *elem) {
	t_test_node *src  = (t_test_node *)elem;
	t_test_node *copy = malloc(sizeof(t_test_node));
	if (copy) {
		copy->id    = src->id;
		copy->value = src->value;
		ill_init(&copy->node, offsetof(t_test_node, node));
	}
	return copy;
}

void test_ill_init(void) {
	t_test_node node;

	ill_init(&node.node, offsetof(t_test_node, node));
	ill_init(NULL, offsetof(t_test_node, node));

	assert(node.node.offset == offsetof(t_test_node, node));
	assert(node.node.next == NULL);

	t_test_node node2;
	ill_init(&node2.node, offsetof(t_test_node, node));
	node.id        = 1;
	node.value     = 42;
	node.node.next = &node2.node;
	node2.id       = 2;
	node2.value    = 84;

	t_test_node *copy = ill_copy(&node.node, duplicate_node);

	assert(copy != NULL);
	assert(copy->id == 1);
	assert(copy->value == 42);
	assert(copy->node.next != NULL);
	assert(ill_copy(&node.node, NULL) == NULL);
	assert(ill_copy(NULL, duplicate_node) == NULL);

	t_test_node *copy_next = (t_test_node *)ill_ntoe(copy->node.next);
	assert(copy_next->id == 2);
	assert(copy_next->value == 84);
	assert(copy_next->node.next == NULL);

	free(copy_next);
	free(copy);
}
