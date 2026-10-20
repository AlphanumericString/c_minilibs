#include <assert.h>

#include "ill_tests.h"

static int cmp_val(const void *elem1, const void *elem2) {
	t_test_dnode *n1 = (t_test_dnode *)elem1;
	t_test_dnode *n2 = (t_test_dnode *)elem2;
	return n1->value - n2->value;
}

static bool has_val_42(void *elem, void *arg) {
	t_test_dnode *n = (t_test_dnode *)elem;
	(void)arg;
	return n->value == 42;
}

NO_COV void test_idl_is(void) {
	t_test_dnode n1, n2, n3;
	t_test_dnode non_chained_node;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&non_chained_node.dnode, offsetof(t_test_dnode, dnode));

	n1.value               = 10;
	n2.value               = 20;
	n3.value               = 30;
	non_chained_node.value = 99;

	assert(idl_is_isolated(&n1.dnode) == true);
	assert(idl_is_isolated(&non_chained_node.dnode) == true);
	n1.dnode = (t_idl){.next   = &n2.dnode,
	                   .prev   = NULL,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n2.dnode = (t_idl){.next   = &n3.dnode,
	                   .prev   = &n1.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n3.dnode = (t_idl){.next   = NULL,
	                   .prev   = &n2.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	assert(idl_is_isolated(&n1.dnode) == false);
	assert(idl_is_isolated(&n3.dnode) == false);
	assert(idl_is_isolated(NULL) == false);

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

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	assert(idl_is_sorted(&n1.dnode, cmp_val) == true);
	n1.value = 30;
	n2.value = 20;
	n3.value = 10;
	assert(idl_is_sorted(&n1.dnode, cmp_val) == true);
	n1.value = 20;
	n2.value = 20;
	n3.value = 20;
	assert(idl_is_sorted(&n1.dnode, cmp_val) == true);
	n1.value = 10;
	n2.value = 20;
	n3.value = 20;
	assert(idl_is_sorted(&n1.dnode, cmp_val) == true);
	n1.value = 20;
	n2.value = 20;
	n3.value = 10;
	assert(idl_is_sorted(&n1.dnode, cmp_val) == true);
	n1.value = 10;
	n2.value = 40;
	n3.value = 30;
	assert(idl_is_sorted(&n1.dnode, cmp_val) == false);

	non_chained_node.value = 999;
	assert(idl_is_sorted(&non_chained_node.dnode, cmp_val) == true);

	assert(idl_is_sorted(&n1.dnode, NULL) == false);
	assert(idl_is_sorted(NULL, cmp_val) == false);

	n3.value               = 42;
	non_chained_node.value = 42;
	assert(idl_has(&n1.dnode, has_val_42, NULL) == true);
	n3.value               = 43;
	non_chained_node.value = 43;
	assert(idl_has(&n1.dnode, has_val_42, NULL) == false);
	n3.value               = 42;
	non_chained_node.value = 42;
	assert(idl_has(NULL, has_val_42, NULL) == false);
	assert(idl_has(&n1.dnode, NULL, NULL) == false);
}
