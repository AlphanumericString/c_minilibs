#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_iter(void) {
	t_cm_test_dnode  n1, n2, n3;
	t_cm_idl        *head  = NULL;
	t_cm_test_dnode *typed = NULL;
	void            *it    = NULL;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);

	assert(idl_iter(head, &it) == false);
	assert(it == NULL);
	assert(idl_iter(NULL, &it) == false);
	assert(idl_iter(NULL, NULL) == false);

	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);

	assert(idl_iter(head, NULL) == false);

	it = NULL;
	assert(idl_iter(head, &it) == true);
	assert(it == &n1);
	assert(idl_iter(head, &it) == true);
	assert(it == &n2);
	assert(idl_iter(head, &it) == true);
	assert(it == &n3);
	assert(idl_iter(head, &it) == false);
	assert(it == NULL);

	// exhausted handles restart from the head on the next call
	assert(idl_iter(head, &it) == true);
	assert(it == &n1);

	// typed iterator usage
	assert(idl_iter(head, (void **)&typed) == true);
	assert(typed == &n1);
	assert(idl_iter(head, (void **)&typed) == true);
	assert(typed == &n2);

	// start from an arbitrary element
	it = &n2;
	assert(idl_iter(head, &it) == true);
	assert(it == &n3);
	assert(idl_iter(head, &it) == false);
	assert(it == NULL);
}
