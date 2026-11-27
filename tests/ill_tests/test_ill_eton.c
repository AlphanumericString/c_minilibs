#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_eton(void) {
	t_cm_test_node node;

	ill_init_node(&node.node, offsetof(t_cm_test_node, node));

	assert(ill_eton(&node, offsetof(t_cm_test_node, node)) == &node.node);
	assert(ill_eton(NULL, offsetof(t_cm_test_node, node)) == NULL);
}
