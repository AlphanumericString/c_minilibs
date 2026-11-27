#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_is_empty(void) {
	t_cm_test_dnode n1;
	t_cm_idl       *head = NULL;

	INIT_DNODE(n1, 10, 1);

	assert(idl_is_empty(NULL) == true);
	assert(idl_is_empty(head) == true);
	idl_push_node(&head, &n1.dnode);
	assert(idl_is_empty(head) == false);
}
