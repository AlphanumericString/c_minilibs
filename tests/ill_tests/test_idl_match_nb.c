#include <assert.h>

#include "ill_tests.h"

NO_COV void test_idl_match_nb(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;
	size_t       matches;

	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);
	INIT_DNODE(n4, 4, 4);

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	idl_push_back(&head, &n4.dnode);

	matches = idl_match_nb(head, cb_idl_is_even, NULL);
	assert(matches == 2);
	matches = idl_match_nb(head, NULL, NULL);
	assert(matches == 0);
	matches = idl_match_nb(NULL, cb_idl_is_even, NULL);
	assert(matches == 0);
}
