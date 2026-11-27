#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_init_node(void) {
	t_cm_test_node node;

	ill_init_node(&node.node, offsetof(t_cm_test_node, node));
	ill_init_node(NULL, offsetof(t_cm_test_node, node));

	assert(node.node.offset == offsetof(t_cm_test_node, node));
	assert(node.node.next == NULL);
}
