#include <assert.h>

#include "ill_tests.h"

static int cmp_val(void *elem, void *arg) {
	t_test_dnode *n   = (t_test_dnode *)elem;
	int           val = *(int *)arg;
	return n->value - val;
}

static int cmp_min_max(void *elem1, void *elem2) {
	t_test_dnode *n1 = (t_test_dnode *)elem1;
	t_test_dnode *n2 = (t_test_dnode *)elem2;
	return n1->value - n2->value;
}

void test_idl_get(void) {
	t_test_dnode n1, n2, n3;
	t_idl       *head = NULL;

	idl_init(&n1.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n2.dnode, offsetof(t_test_dnode, dnode));
	idl_init(&n3.dnode, offsetof(t_test_dnode, dnode));

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	idl_push_back(&head, &n1.dnode);
	idl_push_back(&head, &n2.dnode);
	idl_push_back(&head, &n3.dnode);

	int    val20 = 20;
	t_idl *get20 = idl_get(head, cmp_val, &val20);
	assert(get20 == &n2.dnode);

	int    val40 = 40;
	t_idl *get40 = idl_get(head, cmp_val, &val40);
	assert(get40 == NULL);

	t_idl *get_prev20 = idl_get_prev(head, cmp_val, &val20);
	assert(get_prev20 == &n1.dnode);

	t_test_dnode *min_node = (t_test_dnode *)idl_get_min(head, cmp_min_max);
	assert(min_node == &n1);

	t_test_dnode *max_node = (t_test_dnode *)idl_get_max(head, cmp_min_max);
	assert(max_node == &n3);

	t_test_dnode *kth0 = (t_test_dnode *)idl_get_kth(head, 0);
	assert(kth0 == &n1);

	t_test_dnode *kth1 = (t_test_dnode *)idl_get_kth(head, 1);
	assert(kth1 == &n2);

	t_test_dnode *kth3 = (t_test_dnode *)idl_get_kth(head, 3);
	assert(kth3 == NULL);
}
