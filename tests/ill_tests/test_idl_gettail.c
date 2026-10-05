#include <assert.h>

#include "ill_tests.h"

void test_idl_gethead(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));

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
void test_idl_gettail(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));

	assert(idl_get_tail(head) == NULL);
	assert(idl_get_tail(NULL) == NULL);

	idl_push_back(&head, &n1.dnode);
	assert(idl_get_tail(head) == &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	assert(idl_get_tail(head) == &n2.dnode);
	idl_push_back(&head, &n3.dnode);
	assert(idl_get_tail(head) == &n3.dnode);
	assert(idl_get_tail(&n3.dnode) == &n3.dnode);
	assert(idl_get_tail(&n2.dnode) == &n3.dnode);
	assert(idl_get_tail(&n1.dnode) == &n3.dnode);
}
