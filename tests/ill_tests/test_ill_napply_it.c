#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_ill_napply_it(void) {
	t_cm_test_node n1, n2, n3;
	t_cm_ill      *head = NULL;
	t_cm_it_state  st;
	size_t         stop_at = 1;
	int            data    = 42;

	INIT_NODE(n1, 10, 1);
	INIT_NODE(n2, 20, 2);
	INIT_NODE(n3, 30, 3);

	ill_push_back_node(&head, &n1.node);
	ill_push_back_node(&head, &n2.node);
	ill_push_back_node(&head, &n3.node);

	st = (t_cm_it_state){0};
	assert(ill_napply_it(head, 2, cb_ill_it_record, &st) == &st);
	assert(st.count == 2);
	assert(st.first_pos == 0);
	assert(st.last_pos == 1);
	assert(st.nb_last == 1);
	assert(st.sum == 30);

	st = (t_cm_it_state){0};
	assert(ill_napply_it(head, 999, cb_ill_it_record, &st) == &st);
	assert(st.count == 3);
	assert(st.first_pos == 0);
	assert(st.last_pos == 2);
	assert(st.nb_last == 1);
	assert(st.sum == 60);

	st = (t_cm_it_state){0};
	assert(ill_napply_it(head, 1, cb_ill_it_record, &st) == &st);
	assert(st.count == 1);
	assert(st.first_pos == 0);
	assert(st.last_pos == 0);
	assert(st.nb_last == 1);
	assert(st.sum == 10);

	assert(ill_napply_it(head, 2, cb_ill_it_add_pos, &data) == &data);
	assert(n1.value == 10);
	assert(n2.value == 21);
	assert(n3.value == 30);

	assert(ill_napply_it(head, 3, cb_ill_it_stop, &stop_at) == &stop_at);
	assert(n1.value == 110);
	assert(n2.value == 121);
	assert(n3.value == 30);

	st = (t_cm_it_state){0};
	assert(ill_napply_it(head, 0, cb_ill_it_record, &st) == &st);
	assert(st.count == 0);

	assert(ill_napply_it(NULL, 3, cb_ill_it_record, &st) == &st);
	assert(ill_napply_it(head, 3, NULL, &data) == &data);
}
