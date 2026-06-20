#include "ill_tests.h"

void test_ill_ntoe(void) {
	t_test_node node;

	ill_init(&node.node, offsetof(t_test_node, node));

	t_test_node *elem = ill_ntoe(&node.node);

	assert(elem == &node);

	t_ill *node_ptr = ill_eton(&node, offsetof(t_test_node, node));

	assert(node_ptr == &node.node);
}
