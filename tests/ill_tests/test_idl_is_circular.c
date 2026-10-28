#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_is_circular(void) {
	t_test_dnode n1, n2, n3;
	t_test_dnode non_chained_node;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);
	INIT_DNODE(non_chained_node, 45, 4);

	n1.dnode = (t_idl){.next   = &n2.dnode,
	                   .prev   = NULL,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n2.dnode = (t_idl){.next   = &n3.dnode,
	                   .prev   = &n1.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n3.dnode = (t_idl){.next   = NULL,
	                   .prev   = &n2.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};

	assert(idl_is_circular(&n1.dnode) == false);
	// link fwrd && bwrd
	n1.dnode = (t_idl){.next   = &n2.dnode,
	                   .prev   = &n3.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n2.dnode = (t_idl){.next   = &n3.dnode,
	                   .prev   = &n1.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n3.dnode = (t_idl){.next   = &n1.dnode,
	                   .prev   = &n2.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	assert(idl_is_circular(&n1.dnode) == true);
	assert(idl_is_circular(&n2.dnode) == true);
	assert(idl_is_circular(&n3.dnode) == true);
	// link fwrd
	n1.dnode = (t_idl){.next   = &n2.dnode,
	                   .prev   = &n3.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n2.dnode = (t_idl){.next   = &n3.dnode,
	                   .prev   = &n1.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n3.dnode = (t_idl){.next   = &n1.dnode,
	                   .prev   = NULL,
	                   .offset = offsetof(t_test_dnode, dnode)};
	assert(idl_is_circular(&n1.dnode) == true);
	assert(idl_is_circular(&n2.dnode) == true);
	assert(idl_is_circular(&n3.dnode) == true);

	// link bwrd
	n1.dnode = (t_idl){.next   = NULL,
	                   .prev   = &n3.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n2.dnode = (t_idl){.next   = &n3.dnode,
	                   .prev   = &n1.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n3.dnode = (t_idl){.next   = &n1.dnode,
	                   .prev   = &n2.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	assert(idl_is_circular(&n1.dnode) == true);
	assert(idl_is_circular(&n2.dnode) == true);
	assert(idl_is_circular(&n3.dnode) == true);
	// unlink
	n1.dnode = (t_idl){.next   = &n2.dnode,
	                   .prev   = NULL,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n2.dnode = (t_idl){.next   = &n3.dnode,
	                   .prev   = &n1.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n3.dnode = (t_idl){.next   = NULL,
	                   .prev   = &n2.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};

	non_chained_node.dnode.prev = NULL;
	non_chained_node.dnode.next = &non_chained_node.dnode;
	assert(idl_is_circular(&non_chained_node.dnode) == true);
	non_chained_node.dnode.prev = &non_chained_node.dnode;
	non_chained_node.dnode.next = NULL;
	assert(idl_is_circular(&non_chained_node.dnode) == true);
	non_chained_node.dnode.next = &non_chained_node.dnode;
	non_chained_node.dnode.next = &non_chained_node.dnode;
	assert(idl_is_circular(&non_chained_node.dnode) == true);
	non_chained_node.dnode.prev = NULL;
	non_chained_node.dnode.next = NULL;

	n3.dnode.next = NULL;
	n1.dnode.prev = NULL;
	assert(idl_is_circular(NULL) == false);
}
