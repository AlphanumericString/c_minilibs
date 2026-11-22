#include <assert.h>

#include "ill_tests.h"

#define OFF offsetof(t_cm_test_dnode, dnode)

NO_COV void test_cm_idl_init(void) {
	t_cm_test_dnode n;

	idl_init(&n, OFF);
	assert(n.dnode.offset == OFF);
	assert(n.dnode.next == NULL);
	assert(n.dnode.prev == NULL);
	idl_init(NULL, OFF);
}

NO_COV void test_cm_idl_is_isolated(void) {
	t_cm_test_dnode n1, n2;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	assert(idl_is_isolated(&n1, OFF) == true);
	idl_append(&n1, &n2, OFF);
	assert(idl_is_isolated(&n1, OFF) == false);
	assert(idl_is_isolated(&n2, OFF) == false);
	assert(idl_is_isolated(NULL, OFF) == false);
}

NO_COV void test_cm_idl_append(void) {
	t_cm_test_dnode n1, n2, n3, n4;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	idl_init(&n4, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);

	idl_append(&n2, &n3, OFF);
	assert(n2.dnode.next == &n3.dnode && n3.dnode.prev == &n2.dnode);
	idl_append(&n1, &n4, OFF);
	assert(n1.dnode.next == &n4.dnode && n4.dnode.prev == &n1.dnode);
	assert(n4.dnode.next == &n2.dnode && n2.dnode.prev == &n4.dnode);
	idl_append(&n2, NULL, OFF);
	idl_append(NULL, &n4, OFF);
}

NO_COV void test_cm_idl_nappend(void) {
	t_cm_test_dnode n1, n2, n5, n6;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n5, OFF);
	idl_init(&n6, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_append(&n5, &n6, OFF);

	idl_nappend(&n2, &n5, OFF);
	assert(n2.dnode.next == &n5.dnode && n5.dnode.prev == &n2.dnode);
	assert(n5.dnode.next == &n6.dnode && n6.dnode.prev == &n5.dnode);
	assert(n6.dnode.next == NULL);
	idl_nappend(&n2, NULL, OFF);
	idl_nappend(NULL, &n5, OFF);
}

NO_COV void test_cm_idl_prepend(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);

	idl_prepend(&n2, &n3, OFF);
	assert(n3.dnode.next == &n2.dnode && n2.dnode.prev == &n3.dnode);
	assert(n1.dnode.next == &n3.dnode && n3.dnode.prev == &n1.dnode);
	idl_prepend(&n2, NULL, OFF);
	idl_prepend(NULL, &n2, OFF);
}

NO_COV void test_cm_idl_nprepend(void) {
	t_cm_test_dnode n1, n2, n5, n6;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n5, OFF);
	idl_init(&n6, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_append(&n5, &n6, OFF);

	idl_nprepend(&n2, &n5, OFF);
	assert(n1.dnode.next == &n5.dnode && n5.dnode.prev == &n1.dnode);
	assert(n5.dnode.next == &n6.dnode && n6.dnode.prev == &n5.dnode);
	assert(n6.dnode.next == &n2.dnode && n2.dnode.prev == &n6.dnode);
	idl_nprepend(&n2, NULL, OFF);
	idl_nprepend(NULL, &n5, OFF);
}

NO_COV void test_cm_idl_push(void) {
	t_cm_test_dnode n1, n2;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);

	assert(idl_push(&head, &n2, OFF) == &n2);
	assert(head == &n2.dnode);
	assert(idl_push(&head, &n1, OFF) == &n1);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n2.dnode && n2.dnode.prev == &n1.dnode);
	assert(idl_push(NULL, &n1, OFF) == NULL);
	assert(idl_push(&head, NULL, OFF) == NULL);
}

NO_COV void test_cm_idl_npush(void) {
	t_cm_test_dnode n1, n7, n8;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n7, OFF);
	idl_init(&n8, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_append(&n7, &n8, OFF);

	assert(idl_npush(&head, &n7, OFF) == &n7);
	assert(head == &n7.dnode);
	assert(n8.dnode.next == &n1.dnode && n1.dnode.prev == &n8.dnode);
	assert(idl_npush(&head, NULL, OFF) == NULL);
	assert(idl_npush(NULL, &n7, OFF) == NULL);
}

NO_COV void test_cm_idl_push_back(void) {
	t_cm_test_dnode n1, n2, n3, n4;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	idl_init(&n4, OFF);

	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	assert(idl_push_back(&head, &n4, OFF) == &n1);
	assert(n2.dnode.next == &n4.dnode && n4.dnode.prev == &n2.dnode);
	assert(n4.dnode.next == NULL);
	assert(idl_push_back(NULL, &n3, OFF) == NULL);
	assert(idl_push_back(&head, NULL, OFF) == NULL);
}

NO_COV void test_cm_idl_pop(void) {
	t_cm_test_dnode n1, n2;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	assert(idl_pop(&head) == NULL);
	assert(idl_pop(NULL) == NULL);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	assert(idl_pop(&head) == &n1);
	assert(head == &n2.dnode);
	assert(n2.dnode.prev == NULL);
}

NO_COV void test_cm_idl_pop_back(void) {
	t_cm_test_dnode n1, n2;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	assert(idl_pop_back(&head) == NULL);
	assert(idl_pop_back(NULL) == NULL);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	assert(idl_pop_back(&head) == &n2);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == NULL);
}

NO_COV void test_cm_idl_remove_after(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_push_back(&head, &n3, OFF);

	assert(idl_remove_after(&n2, OFF) == &n3);
	assert(n2.dnode.next == NULL);
	assert(idl_remove_after(&n2, OFF) == NULL);
	assert(idl_remove_after(NULL, OFF) == NULL);
}

NO_COV void test_cm_idl_remove(void) {
	t_cm_test_dnode n1, n2, n3, n4;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	idl_init(&n4, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_push_back(&head, &n3, OFF);

	assert(idl_remove(NULL, &n2, OFF) == false);
	assert(idl_remove(&head, NULL, OFF) == false);
	assert(idl_remove(&head, &n4, OFF) == false);
	assert(idl_remove(&head, &n2, OFF) == true);
	assert(head == &n1.dnode);
	assert(n1.dnode.next == &n3.dnode && n3.dnode.prev == &n1.dnode);
	assert(idl_remove(&head, &n1, OFF) == true);
	assert(head == &n3.dnode);
	assert(n3.dnode.prev == NULL);
	assert(idl_remove(&head, &n3, OFF) == true);
	assert(head == NULL);
}

NO_COV void test_cm_idl_find_prev(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);

	assert(idl_find_prev(head, &n2, OFF) == &n1);
	assert(idl_find_prev(head, &n1, OFF) == NULL);
	assert(idl_find_prev(head, &n3, OFF) == NULL);
	assert(idl_find_prev(head, NULL, OFF) == NULL);
	assert(idl_find_prev(NULL, &n2, OFF) == NULL);
}

NO_COV void test_cm_idl_find(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);

	assert(idl_find(head, &n2, OFF) == &n2);
	assert(idl_find(head, &n3, OFF) == NULL);
	assert(idl_find(head, NULL, OFF) == NULL);
	assert(idl_find(NULL, &n2, OFF) == NULL);
}

NO_COV void test_cm_idl_get_prev(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head  = NULL;
	int             val20 = 20;
	int             val40 = 40;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_push_back(&head, &n3, OFF);

	assert(idl_get_prev(head, cb_idl_cmp_val, &val20) == &n1);
	assert(idl_get_prev(head, cb_idl_cmp_val, &val40) == NULL);
	assert(idl_get_prev(head, NULL, &val40) == NULL);
	assert(idl_get_prev(NULL, cb_idl_cmp_val, &val40) == NULL);
}

NO_COV void test_cm_idl_get(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head  = NULL;
	int             val20 = 20;
	int             val40 = 40;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_push_back(&head, &n3, OFF);

	assert(idl_get(head, cb_idl_cmp_val, &val20) == &n2);
	assert(idl_get(head, cb_idl_cmp_val, &val40) == NULL);
	assert(idl_get(head, NULL, &val40) == NULL);
	assert(idl_get(NULL, cb_idl_cmp_val, &val40) == NULL);
}

NO_COV void test_cm_idl_get_tail(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);

	assert(idl_get_tail(head) == NULL);
	idl_push_back(&head, &n1, OFF);
	assert(idl_get_tail(head) == &n1);
	idl_push_back(&head, &n2, OFF);
	assert(idl_get_tail(head) == &n2);
	idl_push_back(&head, &n3, OFF);
	assert(idl_get_tail(head) == &n3);
	assert(idl_get_tail(NULL) == NULL);
}

NO_COV void test_cm_idl_get_head(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);

	assert(idl_get_head(head) == NULL);
	idl_push(&head, &n1, OFF);
	assert(idl_get_head(head) == &n1);
	idl_push(&head, &n2, OFF);
	assert(idl_get_head(head) == &n2);
	idl_push(&head, &n3, OFF);
	assert(idl_get_head(head) == &n3);
	assert(idl_get_head(NULL) == NULL);
}

NO_COV void test_cm_idl_get_min(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_push_back(&head, &n3, OFF);

	assert(idl_get_min(head, cb_idl_cmp_min_max) == &n1);
	n1.value = 30;
	n3.value = 10;
	assert(idl_get_min(head, cb_idl_cmp_min_max) == &n3);
	assert(idl_get_min(NULL, cb_idl_cmp_min_max) == NULL);
	assert(idl_get_min(head, NULL) == NULL);
}

NO_COV void test_cm_idl_get_max(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	n1.value = 10;
	n2.value = 20;
	n3.value = 30;
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_push_back(&head, &n3, OFF);

	assert(idl_get_max(head, cb_idl_cmp_min_max) == &n3);
	n1.value = 30;
	n3.value = 10;
	assert(idl_get_max(head, cb_idl_cmp_min_max) == &n1);
	assert(idl_get_max(NULL, cb_idl_cmp_min_max) == NULL);
	assert(idl_get_max(head, NULL) == NULL);
}

NO_COV void test_cm_idl_get_kth(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head = NULL;

	idl_init(&n1, OFF);
	idl_init(&n2, OFF);
	idl_init(&n3, OFF);
	idl_push_back(&head, &n1, OFF);
	idl_push_back(&head, &n2, OFF);
	idl_push_back(&head, &n3, OFF);

	assert(idl_get_kth(head, 0) == &n1);
	assert(idl_get_kth(head, 1) == &n2);
	assert(idl_get_kth(head, 3) == NULL);
	assert(idl_get_kth(NULL, 0) == NULL);
}
