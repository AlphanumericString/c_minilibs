#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_remove_after(void) {
	t_cm_test_dnode n1, n2, n3, n4;
	t_cm_idl       *rm_after_res;

	INIT_STD_DNODES(n1, n2, n3, n4);
	LINK_UP_DNODES(&n3.dnode, &n4.dnode);
	LINK_UP_DNODES(&n1.dnode, &n2.dnode);

	rm_after_res = idl_remove_after(&n3.dnode);
	assert(rm_after_res == &n4.dnode);
	assert(n3.dnode.next == NULL);
	assert(idl_remove_after(idl_get_tail(&n3.dnode)) == NULL);
	assert(idl_remove_after(NULL) == NULL);

	LINK_UP_DNODES(&n2.dnode, &n4.dnode) // [1-2-4];
	assert(idl_remove_after(&n1.dnode) == &n2.dnode);
	assert(n1.dnode.next == &n4.dnode && n4.dnode.prev == &n1.dnode);
	assert(n1.dnode.prev == NULL && n4.dnode.next == NULL);
}
