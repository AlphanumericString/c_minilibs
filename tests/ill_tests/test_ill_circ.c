#include <assert.h>

#include "ill_tests.h"

typedef struct s_cm_ill_circ_state {
	int    values[8];
	size_t count;
} t_cm_ill_circ_state;

NO_COV static bool circ_record(void *elem, void *arg) {
	t_cm_test_node      *n = (t_cm_test_node *)elem;
	t_cm_ill_circ_state *s = (t_cm_ill_circ_state *)arg;

	s->values[s->count++] = n->value;
	return (true);
}

NO_COV void test_cm_ill_circ_init(void) {
	t_cm_test_node      n1, n2, n3;
	t_cm_ill           *head = NULL;
	t_cm_ill_circ_state st   = {0};
	void               *it   = NULL;
	size_t              off  = offsetof(t_cm_test_node, node);

	INIT_NODE(n1, 1, 1);
	INIT_NODE(n2, 2, 2);
	INIT_NODE(n3, 3, 3);

	assert(ill_circ_length(NULL) == 0);
	assert(ill_circ_get_head_node(NULL) == NULL);
	assert(ill_circ_get_tail_node(NULL) == NULL);
	assert(ill_circ_get_head(NULL, off) == NULL);
	assert(ill_circ_get_tail(NULL, off) == NULL);
	assert(ill_circ_foreach(NULL, cb_true, NULL) == NULL);

	assert(ill_circ_push_back_node(&head, &n1.node) == &n1.node);
	assert(head == &n1.node);
	assert(n1.node.next == &n1.node);
	assert(ill_circ_length(head) == 1);
	assert(ill_circ_get_head_node(head) == &n1.node);
	assert(ill_circ_get_tail_node(head) == &n1.node);
	assert(ill_is_circular(head) == true);

	assert(ill_circ_push_back_node(&head, &n2.node) == &n2.node);
	assert(head == &n2.node);
	assert(n1.node.next == &n2.node);
	assert(n2.node.next == &n1.node);
	assert(ill_circ_length(head) == 2);

	assert(ill_circ_push_back_node(&head, &n3.node) == &n3.node);
	assert(head == &n3.node);
	assert(n3.node.next == &n1.node);
	assert(n1.node.next == &n2.node);
	assert(n2.node.next == &n3.node);
	assert(ill_circ_length(head) == 3);
	assert(ill_circ_get_head_node(head) == &n1.node);
	assert(ill_circ_get_tail_node(head) == &n3.node);
	assert(ill_circ_get_head(&n3, off) == &n1);
	assert(ill_circ_get_tail(&n3, off) == &n3);

	assert(ill_circ_foreach(head, circ_record, &st) == &st);
	assert(st.count == 3);
	assert(st.values[0] == 1 && st.values[1] == 2 && st.values[2] == 3);
	assert(ill_circ_foreach(head, cb_false, &st) == &st);
	assert(ill_circ_foreach(head, NULL, &st) == &st);

	assert(ill_circ_iter(head, NULL, off) == false);
	st.count = 0;
	while (ill_circ_iter(head, &it, off)) {
		st.values[st.count++] = ((t_cm_test_node *)it)->value;
	}
	assert(st.count == 3);
	assert(st.values[0] == 1 && st.values[1] == 2 && st.values[2] == 3);
	assert(it == NULL);

	assert(ill_circ_iter(head, &it, off) == true);
	assert(it == &n1);
	it = NULL;
	assert(ill_circ_iter(NULL, &it, off) == false);
	assert(it == NULL);
}

NO_COV void test_cm_ill_circ_push_pop(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;
	size_t         off  = offsetof(t_cm_test_node, node);

	INIT_NODE(n1, 1, 1);
	INIT_NODE(n2, 2, 2);
	INIT_NODE(n3, 3, 3);

	assert(ill_circ_push_node(&head, &n1.node) == &n1.node);
	assert(head == &n1.node);
	assert(n1.node.next == &n1.node);

	assert(ill_circ_push_node(&head, &n2.node) == &n1.node);
	assert(head == &n1.node);
	assert(n1.node.next == &n2.node);
	assert(n2.node.next == &n1.node);

	assert(ill_circ_pop_node(&head) == &n2.node);
	assert(head == &n1.node);
	assert(n1.node.next == &n1.node);
	assert(ill_circ_pop_node(&head) == &n1.node);
	assert(head == NULL);
	assert(ill_circ_pop_node(&head) == NULL);

	head = NULL;
	assert(ill_circ_push_back_node(&head, &n1.node) == &n1.node);
	assert(ill_circ_push_back_node(&head, &n2.node) == &n2.node);
	assert(ill_circ_push_back_node(&head, &n3.node) == &n3.node);
	assert(head == &n3.node);

	assert(ill_circ_pop_back_node(&head) == &n3.node);
	assert(head == &n2.node);
	assert(n2.node.next == &n1.node);
	assert(n1.node.next == &n2.node);
	assert(ill_circ_pop_back_node(&head) == &n2.node);
	assert(head == &n1.node);
	assert(n1.node.next == &n1.node);
	assert(ill_circ_pop_back_node(&head) == &n1.node);
	assert(head == NULL);
	assert(ill_circ_pop_back_node(&head) == NULL);

	head = NULL;
	assert(ill_circ_push(&head, &n1, off) == &n1);
	assert(ill_circ_push_back(&head, &n2, off) == &n1);
	assert(ill_circ_push_back(&head, &n3, off) == &n1);
	assert(ill_circ_pop(&head) == &n1);
	assert(ill_circ_pop_back(&head) == &n3);
	assert(ill_circ_pop(&head) == &n2);
	assert(head == NULL);

	assert(ill_circ_push_node(NULL, &n1.node) == NULL);
	assert(ill_circ_push_node(&head, NULL) == NULL);
	assert(ill_circ_push_back_node(NULL, &n1.node) == NULL);
	assert(ill_circ_push_back_node(&head, NULL) == NULL);
	assert(ill_circ_push(NULL, &n1, off) == NULL);
	assert(ill_circ_push_back(NULL, &n1, off) == NULL);
	assert(ill_circ_pop(NULL) == NULL);
	assert(ill_circ_pop_back(NULL) == NULL);
	assert(ill_circ_pop(&head) == NULL);
	assert(ill_circ_pop_back(&head) == NULL);
}

NO_COV void test_cm_ill_circ_concat(void) {
	t_cm_test_node a[3], b[2];
	t_cm_ill      *l1 = NULL, *l2 = NULL, *ret;
	size_t         off = offsetof(t_cm_test_node, node);
	size_t         i;

	for (i = 0; i < 3; i++) {
		INIT_NODE(a[i], (int)(i + 1), (int)i);
		ill_circ_push_back_node(&l1, &a[i].node);
	}
	for (i = 0; i < 2; i++) {
		INIT_NODE(b[i], (int)(i + 4), (int)(i + 3));
		ill_circ_push_back_node(&l2, &b[i].node);
	}

	assert(ill_circ_concat_node(NULL, NULL) == NULL);
	assert(ill_circ_concat_node(l1, NULL) == l1);
	assert(ill_circ_concat_node(NULL, l2) == l2);

	ret = ill_circ_concat_node(l1, l2);
	assert(ret == &b[1].node);
	assert(ill_circ_length(ret) == 5);
	assert(ret->next == &a[0].node);
	assert(a[0].node.next == &a[1].node);
	assert(a[1].node.next == &a[2].node);
	assert(a[2].node.next == &b[0].node);
	assert(b[0].node.next == &b[1].node);
	assert(b[1].node.next == &a[0].node);

	l1 = NULL;
	l2 = NULL;
	for (i = 0; i < 3; i++)
		ill_circ_push_back_node(&l1, &a[i].node);
	for (i = 0; i < 2; i++)
		ill_circ_push_back_node(&l2, &b[i].node);
	assert(ill_circ_concat(&a[2], &b[1], off) == &b[1]);
	assert(ill_circ_concat(NULL, &b[1], off) == &b[1]);
	assert(ill_circ_concat(&a[2], NULL, off) == &a[2]);
	assert(ill_circ_concat(NULL, NULL, off) == NULL);
}

NO_COV void test_cm_ill_circ_remove_clear(void) {
	t_cm_test_node n1, n2, n3, other;
	t_cm_ill      *head = NULL;
	size_t         off  = offsetof(t_cm_test_node, node);

	INIT_NODE(n1, 1, 1);
	INIT_NODE(n2, 2, 2);
	INIT_NODE(n3, 3, 3);
	INIT_NODE(other, 4, 4);

	ill_circ_push_back_node(&head, &n1.node);
	assert(ill_circ_remove_node(&head, NULL) == false);
	assert(ill_circ_remove_node(&head, &other.node) == false);
	assert(ill_circ_remove_node(&head, &n1.node) == true);
	assert(head == NULL);
	assert(n1.node.next == NULL);
	assert(ill_circ_remove_node(NULL, &n1.node) == false);
	assert(ill_circ_remove_node(&head, NULL) == false);
	assert(ill_circ_remove_node(&head, &n1.node) == false);

	ill_circ_push_back_node(&head, &n1.node);
	ill_circ_push_back_node(&head, &n2.node);
	ill_circ_push_back_node(&head, &n3.node);

	assert(ill_circ_remove_node(&head, &n3.node) == true);
	assert(head == &n2.node);
	assert(n2.node.next == &n1.node);
	assert(n1.node.next == &n2.node);

	assert(ill_circ_remove_node(&head, &n1.node) == true);
	assert(head == &n2.node);
	assert(n2.node.next == &n2.node);

	assert(ill_circ_remove_node(&head, &n2.node) == true);
	assert(head == NULL);

	ill_circ_push_back_node(&head, &n1.node);
	ill_circ_push_back_node(&head, &n2.node);
	assert(ill_circ_remove_node(&head, &other.node) == false);
	assert(ill_circ_remove(&head, &n1, off) == true);
	assert(ill_circ_remove(&head, &n2, off) == true);
	assert(head == NULL);
	assert(ill_circ_remove(NULL, &n1, off) == false);
	assert(ill_circ_remove(&head, NULL, off) == false);

	ill_circ_push_back_node(&head, &n1.node);
	ill_circ_push_back_node(&head, &n2.node);
	ill_circ_push_back_node(&head, &n3.node);
	g_dtor_hits = 0;
	ill_circ_clear(&head, cb_count_dtor);
	assert(g_dtor_hits == 3);
	assert(head == NULL);
	assert(n1.node.next == NULL);
	assert(n2.node.next == NULL);
	assert(n3.node.next == NULL);

	ill_circ_clear(&head, cb_count_dtor);
	assert(g_dtor_hits == 3);
	ill_circ_clear(NULL, cb_count_dtor);
}
