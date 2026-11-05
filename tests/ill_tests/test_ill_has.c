#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_has(void) {
	t_cm_test_node n1, n2, n3;

	INIT_NODE(n1, 0, 1);
	INIT_NODE(n2, 0, 2);
	INIT_NODE(n3, 42, 3);

	LINK_UP_NODES(&n1.node, &n2.node);
	LINK_UP_NODES(&n2.node, &n3.node);

	assert(ill_has(&n1.node, cb_ill_has_val_42, NULL) == true);
	n3.value = 43;
	assert(ill_has(&n1.node, cb_ill_has_val_42, NULL) == false);
	assert(ill_has(&n1.node, NULL, NULL) == false);
	assert(ill_has(NULL, cb_ill_has_val_42, NULL) == false);
}
