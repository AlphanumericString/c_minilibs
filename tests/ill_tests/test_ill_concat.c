#include <assert.h>

#include "ill_tests.h"

NO_COV static void build_list(t_cm_ill **head, t_cm_test_node *nodes,
                              size_t n) {
	size_t i;

	*head = NULL;
	for (i = 0; i < n; i++) {
		INIT_NODE(nodes[i], (int)(i + 1), (int)i);
		ill_push_back_node(head, &nodes[i].node);
	}
}

NO_COV void test_cm_ill_concat(void) {
	t_cm_test_node a[3], b[2];
	t_cm_ill      *l1, *l2, *ret;
	size_t         off = offsetof(t_cm_test_node, node);

	build_list(&l1, a, 3);
	build_list(&l2, b, 2);

	assert(ill_concat_node(NULL, NULL) == NULL);
	assert(ill_concat_node(l1, NULL) == l1);
	assert(ill_concat_node(NULL, l2) == l2);

	ret = ill_concat_node(l1, l2);
	assert(ret == l1);
	assert(ill_length(ret) == 5);
	assert(a[0].node.next == &a[1].node);
	assert(a[1].node.next == &a[2].node);
	assert(a[2].node.next == &b[0].node);
	assert(b[0].node.next == &b[1].node);
	assert(b[1].node.next == NULL);

	build_list(&l1, a, 3);
	build_list(&l2, b, 2);
	assert(ill_concat(NULL, NULL, off) == NULL);
	assert(ill_concat(&a[0], NULL, off) == &a[0]);
	assert(ill_concat(NULL, &b[0], off) == &b[0]);
	assert(ill_concat(&a[0], &b[0], off) == &a[0]);
	assert(ill_length(a[0].node.next->next->next) == 2);

	build_list(&l1, a, 2);
	a[0].value = 1;
	a[1].value = 3;
	build_list(&l2, b, 2);
	b[0].value = 2;
	b[1].value = 4;
	assert(ill_merge(l1, l2, NULL) == NULL);
	ret = ill_merge(l1, l2, cb_ill_cmp_min_max);
	assert(ret == &a[0].node);
	assert(a[0].node.next == &b[0].node);
	assert(b[0].node.next == &a[1].node);
	assert(a[1].node.next == &b[1].node);
	assert(b[1].node.next == NULL);
}
