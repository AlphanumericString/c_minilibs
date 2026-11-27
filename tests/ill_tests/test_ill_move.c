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

NO_COV void test_cm_ill_move(void) {
	t_cm_test_node a[5], b[3];
	t_cm_ill      *from, *to, *empty;
	size_t         off = offsetof(t_cm_test_node, node);

	build_list(&from, a, 5);
	build_list(&to, b, 3);
	empty = NULL;

	assert(ill_move(NULL, &to, &a[0], &a[1], off) == false);
	assert(ill_move(&from, NULL, &a[0], &a[1], off) == false);
	assert(ill_move(&empty, &to, &a[0], &a[1], off) == false);
	assert(ill_move(&from, &to, NULL, &a[1], off) == false);
	assert(ill_move(&from, &to, &a[0], NULL, off) == false);
	assert(ill_move_node(&from, &to, NULL, &a[1].node) == false);
	assert(ill_move_node(&from, &to, &a[0].node, NULL) == false);
	assert(ill_move(&from, &from, &a[0], &a[1], off) == false);
	assert(ill_move(&from, &to, &b[0], &a[1], off) == false);
	assert(ill_move(&from, &to, &a[2], &a[1], off) == false);

	// move a middle range, predecessor present, destination non-empty
	assert(ill_move(&from, &to, &a[1], &a[2], off) == true);
	assert(ill_length(from) == 3);
	assert(from == &a[0].node);
	assert(a[0].node.next == &a[3].node);
	assert(a[3].node.next == &a[4].node);
	assert(a[4].node.next == NULL);
	assert(ill_length(to) == 5);
	assert(to == &b[0].node);
	assert(b[2].node.next == &a[1].node);
	assert(a[1].node.next == &a[2].node);
	assert(a[2].node.next == NULL);

	// move a single head node into an empty destination
	build_list(&from, a, 3);
	to = NULL;
	assert(ill_move(&from, &to, &a[0], &a[0], off) == true);
	assert(from == &a[1].node);
	assert(a[1].node.next == &a[2].node);
	assert(a[2].node.next == NULL);
	assert(to == &a[0].node);
	assert(a[0].node.next == NULL);

	// move a single head node into a non-empty destination
	build_list(&from, a, 3);
	build_list(&to, b, 2);
	assert(ill_move(&from, &to, &a[0], &a[0], off) == true);
	assert(from == &a[1].node);
	assert(to == &b[0].node);
	assert(b[1].node.next == &a[0].node);
	assert(a[0].node.next == NULL);

	// move the whole source list
	build_list(&from, a, 3);
	build_list(&to, b, 2);
	assert(ill_move(&from, &to, &a[0], &a[2], off) == true);
	assert(from == NULL);
	assert(to == &b[0].node);
	assert(b[1].node.next == &a[0].node);
	assert(a[2].node.next == NULL);

	// move a tail range
	build_list(&from, a, 5);
	build_list(&to, b, 2);
	assert(ill_move(&from, &to, &a[2], &a[4], off) == true);
	assert(ill_length(from) == 2);
	assert(a[1].node.next == NULL);
	assert(b[1].node.next == &a[2].node);
	assert(a[4].node.next == NULL);
}
