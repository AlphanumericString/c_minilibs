#include <assert.h>

#include "ill_tests.h"

static int cmp_val(void *elem, void *arg) {
	t_test_node *n   = (t_test_node *)elem;
	int          val = *(int *)arg;
	return n->value - val;
}

static int cmp_min_max(void *elem1, void *elem2) {
	t_test_node *n1 = (t_test_node *)elem1;
	t_test_node *n2 = (t_test_node *)elem2;
	return n1->value - n2->value;
}

void test_ill_get(void) {
	t_test_node n1, n2, n3;
	t_ill      *head = NULL;

	ill_init(&n1.node, offsetof(t_test_node, node));
	ill_init(&n2.node, offsetof(t_test_node, node));
	ill_init(&n3.node, offsetof(t_test_node, node));

	n1.value = 10;
	n2.value = 20;
	n3.value = 30;

	ill_push_back(&head, &n1.node);
	ill_push_back(&head, &n2.node);
	ill_push_back(&head, &n3.node);

	int    val20 = 20;
	t_ill *get20 = ill_get(head, cmp_val, &val20);
	assert(get20 == &n2.node);

	int    val40 = 40;
	t_ill *get40 = ill_get(head, cmp_val, &val40);
	assert(get40 == NULL);

	t_ill *get_prev20 = ill_get_prev(head, cmp_val, &val20);
	assert(get_prev20 == &n1.node);

	t_test_node *min_node = (t_test_node *)ill_get_min(head, cmp_min_max);
	assert(min_node == &n1);

	t_test_node *max_node = (t_test_node *)ill_get_max(head, cmp_min_max);
	assert(max_node == &n3);

	t_test_node *kth0 = (t_test_node *)ill_get_kth(head, 0);
	assert(kth0 == &n1);

	t_test_node *kth1 = (t_test_node *)ill_get_kth(head, 1);
	assert(kth1 == &n2);

	t_test_node *kth3 = (t_test_node *)ill_get_kth(head, 3);
	assert(kth3 == NULL);
}
