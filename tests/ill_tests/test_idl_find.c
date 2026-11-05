#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_find(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;
	t_cm_idl       *not_found, *found;

	INIT_DNODE(n1, 42, 1);
	INIT_DNODE(n2, 43, 2);
	INIT_DNODE(n3, 44, 3);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);

	found = idl_find(head, &n2.dnode);
	assert(found == &n2.dnode);

	not_found = idl_find(head, &n3.dnode);
	assert(not_found == NULL);
	not_found = idl_find(NULL, &n3.dnode);
	assert(not_found == NULL);
	not_found = idl_find(head, NULL);
	assert(not_found == NULL);
}
