#include <assert.h>

#include "ill_tests.h"

static int cmp_val(const void *elem, const void *arg) {
	t_test_dnode *n   = (t_test_dnode *)elem;
	int           val = *(int *)arg;
	return n->value - val;
}

static int cmp_min_max(const void *elem1, const void *elem2) {
	t_test_dnode *n1 = (t_test_dnode *)elem1;
	t_test_dnode *n2 = (t_test_dnode *)elem2;
	return n1->value - n2->value;
}

void test_idl_get(void) {
	t_test_dnode n1, n2, n3, n4;
	t_idl       *head = NULL;

	t_idl        *idl_error, *idl_not_found;
	t_idl        *idl_ok;
	t_test_dnode *node_not_found, *node_error;
	t_test_dnode *node_ok;
	int           ok_search_val  = 20;
	int           bad_search_val = 40;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n4.dnode, offsetof(t_test_dnode, dnode));

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	n4.value = 5;

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	idl_ok = idl_get(head, cmp_val, &ok_search_val);
	assert(idl_ok == &n2.dnode);
	idl_not_found = idl_get(head, cmp_val, &bad_search_val);
	assert(idl_not_found == NULL);
	idl_error = idl_get(NULL, cmp_val, &bad_search_val);
	assert(idl_error == NULL);
	idl_error = idl_get(head, NULL, &bad_search_val);
	assert(idl_error == NULL);

	idl_ok = idl_get_prev(head, cmp_val, &ok_search_val);
	assert(idl_ok == &n1.dnode);
	idl_not_found = idl_get_prev(head, cmp_val, &bad_search_val);
	assert(idl_not_found == NULL);
	idl_error = idl_get_prev(NULL, cmp_val, &ok_search_val);
	assert(idl_error == NULL);
	idl_error = idl_get_prev(head, NULL, &ok_search_val);
	assert(idl_error == NULL);

	// introducing badly ordered node for min/max loop grab
	idl_push_back(&head, &n4.dnode);

	node_ok = (t_test_dnode *)idl_get_min(head, cmp_min_max);
	assert(node_ok == &n4);
	node_error = (t_test_dnode *)idl_get_min(head, NULL);
	assert(node_error == NULL);
	node_error = (t_test_dnode *)idl_get_min(NULL, cmp_min_max);
	assert(node_error == NULL);

	node_ok = (t_test_dnode *)idl_get_max(head, cmp_min_max);
	assert(node_ok == &n3);
	node_error = (t_test_dnode *)idl_get_max(head, NULL);
	assert(node_error == NULL);
	node_error = (t_test_dnode *)idl_get_max(NULL, cmp_min_max);
	assert(node_error == NULL);

	node_ok = (t_test_dnode *)idl_get_kth(head, 0);
	assert(node_ok == &n1);
	node_ok = (t_test_dnode *)idl_get_kth(head, 1);
	assert(node_ok == &n2);
	node_not_found = (t_test_dnode *)idl_get_kth(head, 99);
	assert(node_not_found == NULL);
	node_error = (t_test_dnode *)idl_get_kth(NULL, 3);
	assert(node_error == NULL);
}
