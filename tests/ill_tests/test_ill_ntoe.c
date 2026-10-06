#include <assert.h>

#include "ill_tests.h"

void test_ill_ntoe(void) {
	t_test_node node;

	ill_init(&node.node, offsetof(t_test_node, node));

	assert(ill_ntoe(&node.node) == &node);
	assert(ill_ntoe(NULL) == NULL);
	assert(ill_eton(&node, offsetof(t_test_node, node)) == &node.node);
	assert(ill_eton(NULL, offsetof(t_test_node, node)) == NULL);
}
