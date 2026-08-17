#include <assert.h>

#include "ill_tests.h"

static int cmp_val(const void *elem, const void *arg) {
	t_test_node *n   = (t_test_node *)elem;
	int          val = *(int *)arg;
	return n->value - val;
}

static int cmp_min_max(const void *elem1, const void *elem2) {
	t_test_node *n1 = (t_test_node *)elem1;
	t_test_node *n2 = (t_test_node *)elem2;
	return n1->value - n2->value;
}

void test_ill_get(void) {
	t_test_node n1, n2, n3;
	t_ill      *head  = NULL;
	int         val20 = 20;
	int         val40 = 40;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	assert(ill_get(head, cmp_val, &val20) == &n2.node);
	assert(ill_get(head, cmp_val, &val40) == NULL);
	assert(ill_get(head, NULL, &val40) == NULL);
	assert(ill_get(NULL, cmp_val, &val40) == NULL);

	assert(ill_get_prev(head, cmp_val, &val20) == &n1.node);
	assert(ill_get_prev(head, cmp_val, &val40) == NULL);
	assert(ill_get_prev(head, NULL, &val40) == NULL);
	assert(ill_get_prev(NULL, cmp_val, &val40) == NULL);

	assert(ill_get_min(head, cmp_min_max) == &n1);
	assert(ill_get_max(head, cmp_min_max) == &n3);
	n1.value = 30;
	n2.value = 20;
	n3.value = 10;
	assert(ill_get_min(head, cmp_min_max) == &n3);
	assert(ill_get_max(head, cmp_min_max) == &n1);

	assert(ill_get_min(NULL, cmp_min_max) == NULL);
	assert(ill_get_min(head, NULL) == NULL);
	assert(ill_get_max(NULL, cmp_min_max) == NULL);
	assert(ill_get_max(head, NULL) == NULL);

	assert(ill_get_kth(head, 0) == &n1);
	assert(ill_get_kth(head, 1) == &n2);
	assert(ill_get_kth(head, 3) == NULL);
	assert(ill_get_kth(NULL, 0) == NULL);
}
