#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_get_head(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	assert(idl_get_head(head) == NULL);
	assert(idl_get_head(NULL) == NULL);

	idl_push(&head, &n1.dnode);
	assert(idl_get_head(head) == &n1.dnode);

	idl_push(&head, &n2.dnode);
	assert(idl_get_head(head) == &n2.dnode);

	idl_push(&head, &n3.dnode);
	assert(idl_get_head(head) == &n3.dnode);
	assert(idl_get_head(&n3.dnode) == &n3.dnode);
	assert(idl_get_head(&n2.dnode) == &n3.dnode);
	assert(idl_get_head(&n1.dnode) == &n3.dnode);
}
