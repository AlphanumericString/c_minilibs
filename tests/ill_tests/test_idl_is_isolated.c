#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_is_isolated(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_test_dnode non_chained_node;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);
	INIT_DNODE(non_chained_node, 45, 4);

	assert(idl_is_isolated(&n1.dnode) == true);
	assert(idl_is_isolated(&non_chained_node.dnode) == true);
	n1.dnode = (t_cm_idl){.next   = &n2.dnode,
	                      .prev   = NULL,
	                      .offset = offsetof(t_cm_test_dnode, dnode)};
	n2.dnode = (t_cm_idl){.next   = &n3.dnode,
	                      .prev   = &n1.dnode,
	                      .offset = offsetof(t_cm_test_dnode, dnode)};
	n3.dnode = (t_cm_idl){.next   = NULL,
	                      .prev   = &n2.dnode,
	                      .offset = offsetof(t_cm_test_dnode, dnode)};
	assert(idl_is_isolated(&n1.dnode) == false);
	assert(idl_is_isolated(&n3.dnode) == false);
	assert(idl_is_isolated(NULL) == false);
}
