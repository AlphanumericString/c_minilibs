#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_apply_it(void) {
	t_cm_test_dnode n1, n2, n3;
	t_cm_idl       *head    = NULL;
	t_cm_it_state   st      = {0};
	size_t          stop_at = 1;
	int             data    = 42;

	INIT_DNODE(n1, 10, 1);
	INIT_DNODE(n2, 20, 2);
	INIT_DNODE(n3, 30, 3);

	idl_push_back_node(&head, &n1.dnode);
	idl_push_back_node(&head, &n2.dnode);
	idl_push_back_node(&head, &n3.dnode);

	assert(idl_apply_it(head, cb_idl_it_record, &st) == &st);
	assert(st.count == 3);
	assert(st.first_pos == 0);
	assert(st.last_pos == 2);
	assert(st.nb_last == 1);
	assert(st.sum == 60);

	assert(idl_apply_it(head, cb_idl_it_add_pos, &data) == &data);
	assert(n1.value == 10);
	assert(n2.value == 21);
	assert(n3.value == 32);

	assert(idl_apply_it(head, cb_idl_it_stop, &stop_at) == &stop_at);
	assert(n1.value == 110);
	assert(n2.value == 121);
	assert(n3.value == 32);

	assert(idl_apply_it(head, NULL, &data) == &data);
	assert(idl_apply_it(NULL, cb_idl_it_record, &st) == &st);
}
