#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_is_sorted(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_test_node isolated_node;

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);
	INIT_NODE(isolated_node, 0, 0);

	LINK_UP_NODES(&n1.node, &n2.node);
	LINK_UP_NODES(&n2.node, &n3.node);
	assert(ill_is_sorted(&n1.node, cb_ill_cmp_min_max) == true);
	n1.value = 30;
	n2.value = 20;
	n3.value = 10;
	assert(ill_is_sorted(&n1.node, cb_ill_cmp_min_max) == true);
	n1.value = 20;
	n2.value = 20;
	n3.value = 20;
	assert(ill_is_sorted(&n1.node, cb_ill_cmp_min_max) == true);
	assert(ill_is_sorted(&isolated_node.node, cb_ill_cmp_min_max) == true);
	n1.value = 10;
	n2.value = 40;
	n3.value = 30;
	assert(ill_is_sorted(&n1.node, cb_ill_cmp_min_max) == false);
	n1.value = 10;
	n2.value = 0;
	n3.value = 30;
	assert(ill_is_sorted(&n1.node, cb_ill_cmp_min_max) == false);

	assert(ill_is_sorted(&n1.node, NULL) == false);
	assert(ill_is_sorted(NULL, cb_ill_cmp_min_max) == false);
}
