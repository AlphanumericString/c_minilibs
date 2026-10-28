#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_has(void) {
	t_test_dnode n1, n2, n3;

	INIT_DNODE(n1, 0, 1);
	INIT_DNODE(n2, 0, 2);
	INIT_DNODE(n3, 42, 3);

	n1.dnode = (t_idl){.next   = &n2.dnode,
	                   .prev   = NULL,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n2.dnode = (t_idl){.next   = &n3.dnode,
	                   .prev   = &n1.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};
	n3.dnode = (t_idl){.next   = NULL,
	                   .prev   = &n2.dnode,
	                   .offset = offsetof(t_test_dnode, dnode)};

	assert(idl_has(&n1.dnode, cb_idl_has_val_42, NULL) == true);
	n3.value = 43;
	assert(idl_has(&n1.dnode, cb_idl_has_val_42, NULL) == false);
	n3.value = 42;
	assert(idl_has(NULL, cb_idl_has_val_42, NULL) == false);
	assert(idl_has(&n1.dnode, NULL, NULL) == false);
}
