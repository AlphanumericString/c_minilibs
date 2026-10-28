#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_is_sorted(void) {
	t_test_dnode n1, n2, n3;
	t_test_dnode non_chained_node;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);
	INIT_DNODE(non_chained_node, 999, 4);

	n1.dnode = (t_idl){.next   = &n2.dnode,
	                   .prev   = NULL,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n2.dnode = (t_idl){.next   = &n3.dnode,
	                   .prev   = &n1.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n3.dnode = (t_idl){.next   = NULL,
	                   .prev   = &n2.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};

	assert(idl_is_sorted(&n1.dnode, cb_idl_cmp_min_max) == true);
	n1.value = 30;
	n2.value = 20;
	n3.value = 10;
	assert(idl_is_sorted(&n1.dnode, cb_idl_cmp_min_max) == true);
	n1.value = 20;
	n2.value = 20;
	n3.value = 20;
	assert(idl_is_sorted(&n1.dnode, cb_idl_cmp_min_max) == true);
	n1.value = 10;
	n2.value = 20;
	n3.value = 20;
	assert(idl_is_sorted(&n1.dnode, cb_idl_cmp_min_max) == true);
	n1.value = 20;
	n2.value = 20;
	n3.value = 10;
	assert(idl_is_sorted(&n1.dnode, cb_idl_cmp_min_max) == true);
	n1.value = 10;
	n2.value = 40;
	n3.value = 30;
	assert(idl_is_sorted(&n1.dnode, cb_idl_cmp_min_max) == false);

	assert(idl_is_sorted(&non_chained_node.dnode, cb_idl_cmp_min_max) == true);

	assert(idl_is_sorted(&n1.dnode, NULL) == false);
	assert(idl_is_sorted(NULL, cb_idl_cmp_min_max) == false);
}
