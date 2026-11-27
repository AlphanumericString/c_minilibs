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

NO_COV void test_cm_idl_move(void) {
	t_cm_test_dnode a[5], b[3];
	t_cm_idl       *from, *to, *empty;
	size_t          off = offsetof(t_cm_test_dnode, dnode);

	build_list(&from, a, 5);
	build_list(&to, b, 3);
	empty = NULL;

	assert(idl_move(NULL, &to, &a[0], &a[1], off) == false);
	assert(idl_move(&from, NULL, &a[0], &a[1], off) == false);
	assert(idl_move(&empty, &to, &a[0], &a[1], off) == false);
	assert(idl_move(&from, &to, NULL, &a[1], off) == false);
	assert(idl_move(&from, &to, &a[0], NULL, off) == false);
	assert(idl_move_node(&from, &to, NULL, &a[1].dnode) == false);
	assert(idl_move_node(&from, &to, &a[0].dnode, NULL) == false);
	assert(idl_move(&from, &from, &a[0], &a[1], off) == false);
	assert(idl_move(&from, &to, &b[0], &a[1], off) == false);
	assert(idl_move(&from, &to, &a[2], &a[1], off) == false);

	// move a middle range, predecessor present, destination non-empty
	assert(idl_move(&from, &to, &a[1], &a[2], off) == true);
	assert(idl_length(from) == 3);
	assert(from == &a[0].dnode);
	assert(a[0].dnode.next == &a[3].dnode);
	assert(a[3].dnode.prev == &a[0].dnode);
	assert(a[3].dnode.next == &a[4].dnode);
	assert(a[4].dnode.prev == &a[3].dnode);
	assert(a[4].dnode.next == NULL);
	assert(idl_length(to) == 5);
	assert(to == &b[0].dnode);
	assert(b[2].dnode.next == &a[1].dnode);
	assert(a[1].dnode.prev == &b[2].dnode);
	assert(a[1].dnode.next == &a[2].dnode);
	assert(a[2].dnode.prev == &a[1].dnode);
	assert(a[2].dnode.next == NULL);

	// move a single head node into an empty destination
	build_list(&from, a, 3);
	to = NULL;
	assert(idl_move(&from, &to, &a[0], &a[0], off) == true);
	assert(from == &a[1].dnode);
	assert(a[1].dnode.prev == NULL);
	assert(a[1].dnode.next == &a[2].dnode);
	assert(a[2].dnode.prev == &a[1].dnode);
	assert(a[2].dnode.next == NULL);
	assert(to == &a[0].dnode);
	assert(a[0].dnode.prev == NULL);
	assert(a[0].dnode.next == NULL);

	// move a single head node into a non-empty destination
	build_list(&from, a, 3);
	build_list(&to, b, 2);
	assert(idl_move(&from, &to, &a[0], &a[0], off) == true);
	assert(from == &a[1].dnode);
	assert(a[1].dnode.prev == NULL);
	assert(to == &b[0].dnode);
	assert(b[1].dnode.next == &a[0].dnode);
	assert(a[0].dnode.prev == &b[1].dnode);
	assert(a[0].dnode.next == NULL);

	// move the whole source list
	build_list(&from, a, 3);
	build_list(&to, b, 2);
	assert(idl_move(&from, &to, &a[0], &a[2], off) == true);
	assert(from == NULL);
	assert(to == &b[0].dnode);
	assert(b[1].dnode.next == &a[0].dnode);
	assert(a[0].dnode.prev == &b[1].dnode);
	assert(a[2].dnode.next == NULL);

	// move a tail range
	build_list(&from, a, 5);
	build_list(&to, b, 2);
	assert(idl_move(&from, &to, &a[2], &a[4], off) == true);
	assert(idl_length(from) == 2);
	assert(a[0].dnode.next == &a[1].dnode);
	assert(a[1].dnode.prev == &a[0].dnode);
	assert(a[1].dnode.next == NULL);
	assert(b[1].dnode.next == &a[2].dnode);
	assert(a[2].dnode.prev == &b[1].dnode);
	assert(a[4].dnode.next == NULL);
}
