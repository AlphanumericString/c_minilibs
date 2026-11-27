#include <assert.h>

#include "ill_tests.h"

typedef struct s_cm_idl_circ_state {
	int    values[8];
	size_t count;
} t_cm_idl_circ_state;

NO_COV static bool dcirc_record(void *elem, void *arg) {
	t_cm_test_dnode     *n = (t_cm_test_dnode *)elem;
	t_cm_idl_circ_state *s = (t_cm_idl_circ_state *)arg;

	s->values[s->count++] = n->value;
	return (true);
}

NO_COV void test_cm_idl_circ_init(void) {
	t_cm_test_dnode     n1, n2, n3;
	t_cm_idl           *head = NULL;
	t_cm_idl_circ_state st   = {0};
	void               *it   = NULL;
	size_t              off  = offsetof(t_cm_test_dnode, dnode);

	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);

	assert(idl_circ_length(NULL) == 0);
	assert(idl_circ_get_head_node(NULL) == NULL);
	assert(idl_circ_get_tail_node(NULL) == NULL);
	assert(idl_circ_get_head(NULL, off) == NULL);
	assert(idl_circ_get_tail(NULL, off) == NULL);
	assert(idl_circ_foreach(NULL, cb_true, NULL) == NULL);
	assert(idl_circ_foreach_rev(NULL, cb_true, NULL) == NULL);

	assert(idl_circ_push_back_node(&head, &n1.dnode) == &n1.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n1.dnode);
	assert(idl_circ_length(head) == 1);
	assert(idl_circ_get_head_node(head) == &n1.dnode);
	assert(idl_circ_get_tail_node(head) == &n1.dnode);
	assert(idl_is_circular(head) == true);

	assert(idl_circ_push_back_node(&head, &n2.dnode) == &n1.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n1.dnode.prev == &n2.dnode);
	assert(n2.dnode.prev == &n1.dnode);
	assert(n2.dnode.next == &n1.dnode);
	assert(idl_circ_length(head) == 2);

	assert(idl_circ_push_back_node(&head, &n3.dnode) == &n1.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n1.dnode.prev == &n3.dnode);
	assert(n2.dnode.prev == &n1.dnode);
	assert(n2.dnode.next == &n3.dnode);
	assert(n3.dnode.prev == &n2.dnode);
	assert(n3.dnode.next == &n1.dnode);
	assert(idl_circ_length(head) == 3);
	assert(idl_circ_get_head_node(head) == &n1.dnode);
	assert(idl_circ_get_tail_node(head) == &n3.dnode);
	assert(idl_circ_get_head(&n1, off) == &n1);
	assert(idl_circ_get_tail(&n1, off) == &n3);

	assert(idl_circ_foreach(head, dcirc_record, &st) == &st);
	assert(st.count == 3);
	assert(st.values[0] == 1 && st.values[1] == 2 && st.values[2] == 3);

	st.count = 0;
	assert(idl_circ_foreach_rev(head, dcirc_record, &st) == &st);
	assert(st.count == 3);
	assert(st.values[0] == 3 && st.values[1] == 2 && st.values[2] == 1);
	assert(idl_circ_foreach(head, cb_false, &st) == &st);
	assert(idl_circ_foreach_rev(head, cb_false, &st) == &st);

	assert(idl_circ_iter(head, NULL, off) == false);
	assert(idl_circ_iter_rev(head, NULL, off) == false);
	st.count = 0;
	while (idl_circ_iter(head, &it, off)) {
		st.values[st.count++] = ((t_cm_test_dnode *)it)->value;
	}
	assert(st.count == 3);
	assert(st.values[0] == 1 && st.values[1] == 2 && st.values[2] == 3);
	assert(it == NULL);

	st.count = 0;
	while (idl_circ_iter_rev(head, &it, off)) {
		st.values[st.count++] = ((t_cm_test_dnode *)it)->value;
	}
	assert(st.count == 3);
	assert(st.values[0] == 3 && st.values[1] == 2 && st.values[2] == 1);
	assert(it == NULL);

	it = NULL;
	assert(idl_circ_iter(NULL, &it, off) == false);
	assert(it == NULL);
	assert(idl_circ_iter_rev(NULL, &it, off) == false);
	assert(it == NULL);
}

NO_COV void test_cm_idl_circ_push_pop(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;
	size_t          off  = offsetof(t_cm_test_dnode, dnode);

	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);

	assert(idl_circ_push_node(&head, &n1.dnode) == &n1.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n1.dnode);

	assert(idl_circ_push_node(&head, &n2.dnode) == &n2.dnode);
	assert(head == &n2.dnode);
	assert(n2.dnode.next == &n1.dnode);
	assert(n2.dnode.prev == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n1.dnode.prev == &n2.dnode);

	assert(idl_circ_pop_node(&head) == &n2.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n1.dnode);
	assert(idl_circ_pop_node(&head) == &n1.dnode);
	assert(head == NULL);
	assert(idl_circ_pop_node(&head) == NULL);

	head = NULL;
	assert(idl_circ_push_back_node(&head, &n1.dnode) == &n1.dnode);
	assert(idl_circ_push_back_node(&head, &n2.dnode) == &n1.dnode);
	assert(idl_circ_push_back_node(&head, &n3.dnode) == &n1.dnode);
	assert(head == &n1.dnode);

	assert(idl_circ_pop_back_node(&head) == &n3.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode);
	assert(n1.dnode.prev == &n2.dnode);
	assert(n2.dnode.prev == &n1.dnode);
	assert(n2.dnode.next == &n1.dnode);
	assert(idl_circ_pop_back_node(&head) == &n2.dnode);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n1.dnode);
	assert(n1.dnode.prev == &n1.dnode);
	assert(idl_circ_pop_back_node(&head) == &n1.dnode);
	assert(head == NULL);
	assert(idl_circ_pop_back_node(&head) == NULL);

	head = NULL;
	assert(idl_circ_push(&head, &n1, off) == &n1);
	assert(idl_circ_push_back(&head, &n2, off) == &n1);
	assert(idl_circ_push_back(&head, &n3, off) == &n1);
	assert(idl_circ_pop(&head) == &n1);
	assert(idl_circ_pop_back(&head) == &n3);
	assert(idl_circ_pop(&head) == &n2);
	assert(head == NULL);

	assert(idl_circ_push_node(NULL, &n1.dnode) == NULL);
	assert(idl_circ_push_node(&head, NULL) == NULL);
	assert(idl_circ_push_back_node(NULL, &n1.dnode) == NULL);
	assert(idl_circ_push_back_node(&head, NULL) == NULL);
	assert(idl_circ_push(NULL, &n1, off) == NULL);
	assert(idl_circ_push_back(NULL, &n1, off) == NULL);
	assert(idl_circ_pop(NULL) == NULL);
	assert(idl_circ_pop_back(NULL) == NULL);
	assert(idl_circ_pop(&head) == NULL);
	assert(idl_circ_pop_back(&head) == NULL);
}

NO_COV void test_cm_idl_circ_concat(void) {
	t_cm_test_dnode a[3], b[2];
	t_cm_idl       *l1 = NULL, *l2 = NULL, *ret;
	size_t          off = offsetof(t_cm_test_dnode, dnode);
	size_t          i;

	for (i = 0; i < 3; i++) {
		INIT_DNODE(a[i], (int)(i + 1), (int)i);
		idl_circ_push_back_node(&l1, &a[i].dnode);
	}
	for (i = 0; i < 2; i++) {
		INIT_DNODE(b[i], (int)(i + 4), (int)(i + 3));
		idl_circ_push_back_node(&l2, &b[i].dnode);
	}

	assert(idl_circ_concat_node(NULL, NULL) == NULL);
	assert(idl_circ_concat_node(l1, NULL) == l1);
	assert(idl_circ_concat_node(NULL, l2) == l2);

	ret = idl_circ_concat_node(l1, l2);
	assert(ret == &a[0].dnode);
	assert(idl_circ_length(ret) == 5);
	assert(ret->next == &a[1].dnode);
	assert(a[0].dnode.prev == &b[1].dnode);
	assert(a[1].dnode.prev == &a[0].dnode);
	assert(a[2].dnode.prev == &a[1].dnode);
	assert(b[0].dnode.prev == &a[2].dnode);
	assert(b[1].dnode.prev == &b[0].dnode);
	assert(b[1].dnode.next == &a[0].dnode);

	l1 = NULL;
	l2 = NULL;
	for (i = 0; i < 3; i++)
		idl_circ_push_back_node(&l1, &a[i].dnode);
	for (i = 0; i < 2; i++)
		idl_circ_push_back_node(&l2, &b[i].dnode);
	assert(idl_circ_concat(&a[0], &b[0], off) == &a[0]);
	assert(idl_circ_concat(NULL, &b[0], off) == &b[0]);
	assert(idl_circ_concat(&a[0], NULL, off) == &a[0]);
}

NO_COV void test_cm_idl_circ_remove_clear(void) {
	t_cm_test_dnode n1, n2, n3, other;
	t_cm_idl       *head = NULL;
	size_t          off  = offsetof(t_cm_test_dnode, dnode);

	INIT_DNODE(n1, 1, 1);
	INIT_DNODE(n2, 2, 2);
	INIT_DNODE(n3, 3, 3);
	INIT_DNODE(other, 4, 4);

	idl_circ_push_back_node(&head, &n1.dnode);
	assert(idl_circ_remove_node(&head, &n1.dnode) == true);
	assert(head == NULL);
	assert(n1.dnode.next == NULL);
	assert(n1.dnode.prev == NULL);
	assert(idl_circ_remove_node(NULL, &n1.dnode) == false);
	assert(idl_circ_remove_node(&head, NULL) == false);
	assert(idl_circ_remove_node(&head, &n1.dnode) == false);

	idl_circ_push_back_node(&head, &n1.dnode);
	other.dnode.prev = &other.dnode;
	other.dnode.next = &other.dnode;
	assert(idl_circ_remove_node(&head, &other.dnode) == false);
	other.dnode.prev = NULL;
	other.dnode.next = NULL;
	assert(idl_circ_remove_node(&head, &n1.dnode) == true);
	assert(head == NULL);

	idl_circ_push_back_node(&head, &n1.dnode);
	idl_circ_push_back_node(&head, &n2.dnode);
	idl_circ_push_back_node(&head, &n3.dnode);

	assert(idl_circ_remove_node(&head, &n1.dnode) == true);
	assert(head == &n2.dnode);
	assert(n2.dnode.prev == &n3.dnode);
	assert(n3.dnode.next == &n2.dnode);

	assert(idl_circ_remove_node(&head, &n3.dnode) == true);
	assert(head == &n2.dnode);
	assert(n2.dnode.next == &n2.dnode);
	assert(n2.dnode.prev == &n2.dnode);

	assert(idl_circ_remove_node(&head, &n2.dnode) == true);
	assert(head == NULL);

	idl_circ_push_back_node(&head, &n1.dnode);
	idl_circ_push_back_node(&head, &n2.dnode);
	assert(idl_circ_remove_node(&head, &other.dnode) == false);
	assert(idl_circ_remove(&head, &n1, off) == true);
	assert(idl_circ_remove(&head, &n2, off) == true);
	assert(head == NULL);
	assert(idl_circ_remove(NULL, &n1, off) == false);
	assert(idl_circ_remove(&head, NULL, off) == false);

	idl_circ_push_back_node(&head, &n1.dnode);
	idl_circ_push_back_node(&head, &n2.dnode);
	idl_circ_push_back_node(&head, &n3.dnode);
	g_dtor_hits = 0;
	idl_circ_clear(&head, cb_count_dtor);
	assert(g_dtor_hits == 3);
	assert(head == NULL);
	assert(n1.dnode.next == NULL);
	assert(n2.dnode.next == NULL);
	assert(n3.dnode.next == NULL);

	idl_circ_clear(&head, cb_count_dtor);
	assert(g_dtor_hits == 3);
	idl_circ_clear(NULL, cb_count_dtor);
}
