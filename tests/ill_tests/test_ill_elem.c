#include <assert.h>

#include "ill_tests.h"

#define OFF offsetof(t_cm_test_node, node)

NO_COV void test_cm_ill_init(void) {
	t_cm_test_node n;

	ill_init(&n, OFF);
	assert(n.node.offset == OFF);
	assert(n.node.next == NULL);
	ill_init(NULL, OFF);
}

NO_COV void test_cm_ill_is_isolated(void) {
	t_cm_test_node n1, n2;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	assert(ill_is_isolated(&n1, OFF) == true);
	n1.node.next = &n2.node;
	assert(ill_is_isolated(&n1, OFF) == false);
	assert(ill_is_isolated(NULL, OFF) == false);
}

NO_COV void test_cm_ill_append(void) {
	t_cm_test_node n1, n2, n3, n4;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	ill_init(&n4, OFF);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	ill_push_back(&head, &n4, OFF);

	ill_append(&n2, &n3, OFF);
	ASSERT_ORDER_ILL(&n2.node, &n3.node);
	ASSERT_ORDER_ILL(&n3.node, &n4.node);
	ill_append(NULL, &n3, OFF);
	ill_append(&n2, NULL, OFF);
}

NO_COV void test_cm_ill_nappend(void) {
	t_cm_test_node n1, n2, n5, n6;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n5, OFF);
	ill_init(&n6, OFF);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	n5.node.next = &n6.node;

	ill_nappend(&n2, &n5, OFF);
	ASSERT_ORDER_ILL(&n2.node, &n5.node);
	ASSERT_ORDER_ILL(&n5.node, &n6.node);
	assert(n6.node.next == NULL);
	ill_nappend(&n2, NULL, OFF);
	ill_nappend(NULL, &n5, OFF);
}

NO_COV void test_cm_ill_push(void) {
	t_cm_test_node n1, n2;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);

	assert(ill_push(&head, &n2, OFF) == &n2);
	assert(head == &n2.node);
	assert(ill_push(&head, &n1, OFF) == &n1);
	assert(head == &n1.node);
	ASSERT_ORDER_ILL(&n1.node, &n2.node);
	assert(ill_push(NULL, &n1, OFF) == NULL);
	assert(ill_push(&head, NULL, OFF) == NULL);
}

NO_COV void test_cm_ill_npush(void) {
	t_cm_test_node n1, n7, n8;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n7, OFF);
	ill_init(&n8, OFF);
	ill_push_back(&head, &n1, OFF);
	n7.node.next = &n8.node;

	assert(ill_npush(&head, &n7, OFF) == &n7);
	assert(head == &n7.node);
	assert(n8.node.next == &n1.node);
	assert(ill_npush(&head, NULL, OFF) == NULL);
	assert(ill_npush(NULL, &n7, OFF) == NULL);
}

NO_COV void test_cm_ill_push_back(void) {
	t_cm_test_node n1, n2, n3, n4;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	ill_init(&n4, OFF);

	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	assert(ill_push_back(&head, &n4, OFF) == &n1);
	ASSERT_ORDER_ILL(&n2.node, &n4.node);
	assert(n4.node.next == NULL);
	assert(ill_push_back(NULL, &n3, OFF) == NULL);
	assert(ill_push_back(&head, NULL, OFF) == NULL);
}

NO_COV void test_cm_ill_pop(void) {
	t_cm_test_node n1, n2;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	assert(ill_pop(&head) == NULL);
	assert(ill_pop(NULL) == NULL);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	assert(ill_pop(&head) == &n1);
	assert(head == &n2.node);
}

NO_COV void test_cm_ill_pop_back(void) {
	t_cm_test_node n1, n2;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	assert(ill_pop_back(&head) == NULL);
	assert(ill_pop_back(NULL) == NULL);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	assert(ill_pop_back(&head) == &n2);
	assert(head == &n1.node);
	assert(n1.node.next == NULL);
}

NO_COV void test_cm_ill_remove_after(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	ill_push_back(&head, &n3, OFF);

	assert(ill_remove_after(&n2, OFF) == &n3);
	assert(n2.node.next == NULL);
	assert(ill_remove_after(&n2, OFF) == NULL);
	assert(ill_remove_after(NULL, OFF) == NULL);
}

NO_COV void test_cm_ill_remove(void) {
	t_cm_test_node n1, n2, n3, n4;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	ill_init(&n4, OFF);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	ill_push_back(&head, &n3, OFF);

	assert(ill_remove(NULL, &n2, OFF) == false);
	assert(ill_remove(&head, NULL, OFF) == false);
	assert(ill_remove(&head, &n4, OFF) == false);
	assert(ill_remove(&head, &n2, OFF) == true);
	assert(head == &n1.node);
	assert(n1.node.next == &n3.node);
	assert(ill_remove(&head, &n1, OFF) == true);
	assert(head == &n3.node);
	assert(ill_remove(&head, &n3, OFF) == true);
	assert(head == NULL);
}

NO_COV void test_cm_ill_find_prev(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);

	assert(ill_find_prev(head, &n2, OFF) == &n1);
	assert(ill_find_prev(head, &n1, OFF) == NULL);
	assert(ill_find_prev(head, &n3, OFF) == NULL);
	assert(ill_find_prev(head, NULL, OFF) == NULL);
	assert(ill_find_prev(NULL, &n2, OFF) == NULL);
}

NO_COV void test_cm_ill_find(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);

	assert(ill_find(head, &n2, OFF) == &n2);
	assert(ill_find(head, &n3, OFF) == NULL);
	assert(ill_find(head, NULL, OFF) == NULL);
	assert(ill_find(NULL, &n2, OFF) == NULL);
}

NO_COV void test_cm_ill_get_prev(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head  = NULL;
	int            val20 = 20;
	int            val40 = 40;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	ill_push_back(&head, &n3, OFF);

	assert(ill_get_prev(head, cb_ill_cmp_val, &val20) == &n1);
	assert(ill_get_prev(head, cb_ill_cmp_val, &val40) == NULL);
	assert(ill_get_prev(head, NULL, &val40) == NULL);
	assert(ill_get_prev(NULL, cb_ill_cmp_val, &val40) == NULL);
}

NO_COV void test_cm_ill_get(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head  = NULL;
	int            val20 = 20;
	int            val40 = 40;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	ill_push_back(&head, &n3, OFF);

	assert(ill_get(head, cb_ill_cmp_val, &val20) == &n2);
	assert(ill_get(head, cb_ill_cmp_val, &val40) == NULL);
	assert(ill_get(head, NULL, &val40) == NULL);
	assert(ill_get(NULL, cb_ill_cmp_val, &val40) == NULL);
}

NO_COV void test_cm_ill_get_tail(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);

	assert(ill_get_tail(NULL, OFF) == NULL);
	ill_push_back(&head, &n1, OFF);
	assert(ill_get_tail(&n1, OFF) == &n1);
	ill_push_back(&head, &n2, OFF);
	assert(ill_get_tail(&n1, OFF) == &n2);
	ill_push_back(&head, &n3, OFF);
	assert(ill_get_tail(&n3, OFF) == &n3);
	assert(ill_get_tail(NULL, OFF) == NULL);
}

NO_COV void test_cm_ill_get_min(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	ill_push_back(&head, &n3, OFF);

	assert(ill_get_min(head, cb_ill_cmp_min_max) == &n1);
	n1.value = 30;
	n3.value = 10;
	assert(ill_get_min(head, cb_ill_cmp_min_max) == &n3);
	assert(ill_get_min(NULL, cb_ill_cmp_min_max) == NULL);
	assert(ill_get_min(head, NULL) == NULL);
}

NO_COV void test_cm_ill_get_max(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	ill_push_back(&head, &n3, OFF);

	assert(ill_get_max(head, cb_ill_cmp_min_max) == &n3);
	n1.value = 30;
	n3.value = 10;
	assert(ill_get_max(head, cb_ill_cmp_min_max) == &n1);
	assert(ill_get_max(NULL, cb_ill_cmp_min_max) == NULL);
	assert(ill_get_max(head, NULL) == NULL);
}

NO_COV void test_cm_ill_get_kth(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;

	ill_init(&n1, OFF);
	ill_init(&n2, OFF);
	ill_init(&n3, OFF);
	ill_push_back(&head, &n1, OFF);
	ill_push_back(&head, &n2, OFF);
	ill_push_back(&head, &n3, OFF);

	assert(ill_get_kth(head, 0) == &n1);
	assert(ill_get_kth(head, 1) == &n2);
	assert(ill_get_kth(head, 3) == NULL);
	assert(ill_get_kth(NULL, 0) == NULL);
}
