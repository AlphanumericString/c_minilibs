#define C_MINI__IDL_INTERNAL
#define C_MINI__ILL_INTERNAL
#include <assert.h>

#include "ill_tests.h"

NO_COV void test_cm_idl_internal(void) {
	t_cm_test_dnode n[4];
	t_cm_idl       *head;
	t_cm_idl       *tail;
	t_cm_idl       *h1;
	t_cm_idl       *t1;
	t_cm_idl       *h2;
	t_cm_idl       *t2;
	size_t          i;

	for (i = 0; i < 4; i++)
		INIT_DNODE(n[i], (int)i, (int)i);

	// push_back: invalid arguments, empty list, then append
	head = NULL;
	tail = NULL;
	assert(idli_push_back_node(NULL, &tail, &n[0].dnode) == NULL);
	assert(idli_push_back_node(&head, NULL, &n[0].dnode) == NULL);
	assert(idli_push_back_node(&head, &tail, NULL) == NULL);
	assert(idli_push_back_node(&head, &tail, &n[0].dnode) == &n[0].dnode);
	assert(idli_push_back_node(&head, &tail, &n[1].dnode) == &n[0].dnode);
	assert(head == &n[0].dnode && tail == &n[1].dnode);
	assert(head->next == &n[1].dnode && tail->next == NULL);

	// pop_back: invalid arguments and both list shapes
	assert(idli_pop_back_node(NULL, &tail) == NULL);
	assert(idli_pop_back_node(&head, NULL) == NULL);
	h1 = NULL;
	t1 = NULL;
	assert(idli_pop_back_node(&h1, &t1) == NULL);
	assert(idli_pop_back_node(&head, &tail) == &n[1].dnode);
	assert(tail == &n[0].dnode);
	assert(idli_pop_back_node(&head, &tail) == &n[0].dnode);
	assert(head == NULL && tail == NULL);

	// concat: invalid arguments, empty second, empty first, both non-empty
	h1 = &n[0].dnode;
	t1 = &n[0].dnode;
	h2 = &n[1].dnode;
	t2 = &n[1].dnode;
	assert(idli_concat_node(NULL, &t1, &h2, &t2) == NULL);
	assert(idli_concat_node(&h1, NULL, &h2, &t2) == NULL);
	assert(idli_concat_node(&h1, &t1, NULL, &t2) == NULL);
	assert(idli_concat_node(&h1, &t1, &h2, NULL) == NULL);
	h1 = &n[0].dnode;
	t1 = &n[0].dnode;
	h2 = NULL;
	t2 = NULL;
	assert(idli_concat_node(&h1, &t1, &h2, &t2) == &n[0].dnode);
	h1 = NULL;
	t1 = NULL;
	h2 = &n[1].dnode;
	t2 = &n[1].dnode;
	assert(idli_concat_node(&h1, &t1, &h2, &t2) == &n[1].dnode);
	assert(h1 == &n[1].dnode && t1 == &n[1].dnode);
	assert(h2 == NULL && t2 == NULL);
	h1 = &n[0].dnode;
	t1 = &n[0].dnode;
	h2 = &n[1].dnode;
	t2 = &n[1].dnode;
	assert(idli_concat_node(&h1, &t1, &h2, &t2) == &n[0].dnode);
	assert(t1 == &n[1].dnode && h2 == NULL);

	// remove: invalid arguments, sole node, foreign node, tail node
	head = NULL;
	tail = NULL;
	assert(idli_remove_node(NULL, &tail, &n[0].dnode) == NULL);
	assert(idli_remove_node(&head, NULL, &n[0].dnode) == NULL);
	assert(idli_remove_node(&head, &tail, &n[0].dnode) == NULL);
	assert(idli_remove_node(&head, &tail, NULL) == NULL);
	head = &n[0].dnode;
	tail = &n[0].dnode;
	assert(idli_remove_node(&head, &tail, NULL) == NULL);
	n[0].dnode.prev = NULL;
	n[0].dnode.next = NULL;
	assert(idli_remove_node(&head, &tail, &n[0].dnode) == &n[0].dnode);
	assert(head == NULL && tail == NULL);
	head = &n[0].dnode;
	tail = &n[1].dnode;
	LINK_UP_DNODES(&n[0].dnode, &n[1].dnode);
	assert(idli_remove_node(&head, &tail, &n[2].dnode) == NULL);
	assert(idli_remove_node(&head, &tail, &n[1].dnode) == &n[1].dnode);
	assert(head == &n[0].dnode && tail == &n[0].dnode);

	// remove a middle node, relinking its neighbours
	head = &n[0].dnode;
	tail = &n[2].dnode;
	LINK_UP_DNODES(&n[0].dnode, &n[1].dnode);
	LINK_UP_DNODES(&n[1].dnode, &n[2].dnode);
	assert(idli_remove_node(&head, &tail, &n[1].dnode) == &n[1].dnode);
	assert(head == &n[0].dnode && tail == &n[2].dnode);
	assert(n[0].dnode.next == &n[2].dnode);
	assert(n[2].dnode.prev == &n[0].dnode);
}

NO_COV void test_cm_ill_internal(void) {
	t_cm_test_node n[4];
	t_cm_ill      *head;
	t_cm_ill      *tail;
	t_cm_ill      *h1;
	t_cm_ill      *t1;
	t_cm_ill      *h2;
	t_cm_ill      *t2;
	size_t         i;

	for (i = 0; i < 4; i++)
		INIT_NODE(n[i], (int)i, (int)i);

	// push_back: invalid arguments, empty list, then append
	head = NULL;
	tail = NULL;
	assert(illi_push_back_node(NULL, &tail, &n[0].node) == NULL);
	assert(illi_push_back_node(&head, NULL, &n[0].node) == NULL);
	assert(illi_push_back_node(&head, &tail, NULL) == NULL);
	assert(illi_push_back_node(&head, &tail, &n[0].node) == &n[0].node);
	assert(illi_push_back_node(&head, &tail, &n[1].node) == &n[0].node);
	assert(head == &n[0].node && tail == &n[1].node);
	assert(head->next == &n[1].node && tail->next == NULL);

	// pop_back: invalid arguments, sole node, then a longer list
	assert(illi_pop_back_node(NULL, &tail) == NULL);
	assert(illi_pop_back_node(&head, NULL) == NULL);
	h1 = NULL;
	t1 = NULL;
	assert(illi_pop_back_node(&h1, &t1) == NULL);
	assert(illi_pop_back_node(&head, &tail) == &n[1].node);
	assert(head == &n[0].node && tail == &n[0].node);
	assert(illi_pop_back_node(&head, &tail) == &n[0].node);
	assert(head == NULL && tail == NULL);

	// concat: invalid arguments, empty second, empty first, both non-empty
	h1 = &n[0].node;
	t1 = &n[0].node;
	h2 = &n[1].node;
	t2 = &n[1].node;
	assert(illi_concat_node(NULL, &t1, &h2, &t2) == NULL);
	assert(illi_concat_node(&h1, NULL, &h2, &t2) == NULL);
	assert(illi_concat_node(&h1, &t1, NULL, &t2) == NULL);
	assert(illi_concat_node(&h1, &t1, &h2, NULL) == NULL);
	h1 = &n[0].node;
	t1 = &n[0].node;
	h2 = NULL;
	t2 = NULL;
	assert(illi_concat_node(&h1, &t1, &h2, &t2) == &n[0].node);
	h1 = NULL;
	t1 = NULL;
	h2 = &n[1].node;
	t2 = &n[1].node;
	assert(illi_concat_node(&h1, &t1, &h2, &t2) == &n[1].node);
	assert(h1 == &n[1].node && t1 == &n[1].node);
	assert(h2 == NULL && t2 == NULL);
	h1 = &n[0].node;
	t1 = &n[0].node;
	h2 = &n[1].node;
	t2 = &n[1].node;
	assert(illi_concat_node(&h1, &t1, &h2, &t2) == &n[0].node);
	assert(t1 == &n[1].node && h2 == NULL);

	// remove: invalid arguments, sole node, foreign node, tail node
	head = NULL;
	tail = NULL;
	assert(illi_remove_node(NULL, &tail, &n[0].node) == NULL);
	assert(illi_remove_node(&head, NULL, &n[0].node) == NULL);
	assert(illi_remove_node(&head, &tail, &n[0].node) == NULL);
	assert(illi_remove_node(&head, &tail, NULL) == NULL);
	head = &n[0].node;
	tail = &n[0].node;
	assert(illi_remove_node(&head, &tail, NULL) == NULL);
	n[0].node.next = NULL;
	assert(illi_remove_node(&head, &tail, &n[0].node) == &n[0].node);
	assert(head == NULL && tail == NULL);
	head = &n[0].node;
	tail = &n[2].node;
	LINK_UP_NODES(&n[0].node, &n[1].node);
	LINK_UP_NODES(&n[1].node, &n[2].node);
	assert(illi_remove_node(&head, &tail, &n[3].node) == NULL);
	assert(illi_remove_node(&head, &tail, &n[1].node) == &n[1].node);
	assert(head == &n[0].node && tail == &n[2].node);
	assert(illi_remove_node(&head, &tail, &n[0].node) == &n[0].node);
	assert(head == &n[2].node && tail == &n[2].node);
}
