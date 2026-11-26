#include <assert.h>

#include "ill_tests.h"

NO_COV static void build_list(t_cm_idl **head, t_cm_test_dnode *nodes,
                              size_t n) {
	size_t i;

	*head = NULL;
	for (i = 0; i < n; i++) {
		INIT_DNODE(nodes[i], (int)(i + 1), (int)i);
		idl_push_back_node(head, &nodes[i].dnode);
	}
}

NO_COV void test_cm_idl_concat(void) {
	t_cm_test_dnode a[3], b[2];
	t_cm_idl       *l1, *l2, *ret;
	size_t          off = offsetof(t_cm_test_dnode, dnode);

	build_list(&l1, a, 3);
	build_list(&l2, b, 2);

	assert(idl_concat_node(NULL, NULL) == NULL);
	assert(idl_concat_node(l1, NULL) == l1);
	assert(idl_concat_node(NULL, l2) == l2);

	ret = idl_concat_node(l1, l2);
	assert(ret == l1);
	assert(idl_length(ret) == 5);
	assert(a[0].dnode.next == &a[1].dnode);
	assert(a[1].dnode.next == &a[2].dnode);
	assert(a[2].dnode.next == &b[0].dnode);
	assert(b[0].dnode.next == &b[1].dnode);
	assert(b[1].dnode.next == NULL);
	assert(a[0].dnode.prev == NULL);
	assert(b[0].dnode.prev == &a[2].dnode);
	assert(b[1].dnode.prev == &b[0].dnode);

	build_list(&l1, a, 3);
	build_list(&l2, b, 2);
	assert(idl_concat(NULL, NULL, off) == NULL);
	assert(idl_concat(&a[0], NULL, off) == &a[0]);
	assert(idl_concat(NULL, &b[0], off) == &b[0]);
	assert(idl_concat(&a[0], &b[0], off) == &a[0]);
	assert(idl_length(a[0].dnode.next->next->next) == 2);

	build_list(&l1, a, 2);
	a[0].value = 1;
	a[1].value = 3;
	build_list(&l2, b, 2);
	b[0].value = 2;
	b[1].value = 4;
	assert(idl_merge(l1, l2, NULL) == NULL);
	ret = idl_merge(l1, l2, cb_idl_cmp_min_max);
	assert(ret == &a[0].dnode);
	assert(a[0].dnode.next == &b[0].dnode);
	assert(b[0].dnode.next == &a[1].dnode);
	assert(a[1].dnode.next == &b[1].dnode);
	assert(b[1].dnode.next == NULL);
	assert(a[0].dnode.prev == NULL);
	assert(b[0].dnode.prev == &a[0].dnode);
	assert(a[1].dnode.prev == &b[0].dnode);
	assert(b[1].dnode.prev == &a[1].dnode);
}
