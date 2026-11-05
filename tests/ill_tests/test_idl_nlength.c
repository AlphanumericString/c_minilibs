#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_nlength(void) {
	t_cm_test_dnode n1, n2, n3, n4;
	t_cm_idl       *head = NULL;

	INIT_STD_DNODES(n1, n2, n3, n4);

	assert(idl_nlength(head, 10) == 0);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	assert(idl_nlength(head, 2) == 2);
	assert(idl_nlength(head, 10) == 4);
}
