#include <assert.h>
#include <stdlib.h>

#include "ill_tests.h"

NO_COV void test_ill_copy(void) {
	t_test_node  node;
	t_test_node  node2;
	t_test_node *copy;
	t_test_node *copy_next;

	INIT_NODE(node, 42, 1);
	INIT_NODE(node2, 84, 2);
	node.node.next = &node2.node;

	copy = ill_copy(&node.node, cb_ill_duplicate);

	assert(copy != NULL);
	assert(copy->id == 1);
	assert(copy->value == 42);
	assert(copy->node.next != NULL);
	assert(ill_copy(&node.node, NULL) == NULL);
	assert(ill_copy(NULL, cb_ill_duplicate) == NULL);

	copy_next = (t_test_node *)ill_ntoe(copy->node.next);
	assert(copy_next->id == 2);
	assert(copy_next->value == 84);
	assert(copy_next->node.next == NULL);

	free(copy_next);
	free(copy);
}
