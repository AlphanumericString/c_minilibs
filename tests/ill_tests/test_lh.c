#include <assert.h>

#include "ill_tests.h"

typedef struct s_cm_lh_prefix {
	int id;
	int value;
} t_cm_lh_prefix;

typedef struct s_cm_lh_state {
	int    values[8];
	size_t count;
} t_cm_lh_state;

NO_COV static bool lh_record(void *elem, void *arg) {
	t_cm_lh_state *s = (t_cm_lh_state *)arg;

	s->values[s->count++] = ((t_cm_lh_prefix *)elem)->value;
	return (true);
}

NO_COV void test_cm_lh_create(void) {
	t_cm_lh  lh;
	t_cm_lh *heap;
	void    *it = (void *)1;

	assert(lh_init(NULL, CM_LH_SINGLY, 0) == NULL);
	assert(lh_init(&lh, (t_cm_lh_type)99, 0) == NULL);

	assert(lh_init(&lh, CM_LH_SINGLY, 0) == &lh);
	assert(lh_is_valid(&lh) == true);
	assert(lh_is_empty(&lh) == true);
	assert(lh_length(&lh) == 0);
	assert(lh_get_head(&lh) == NULL);
	assert(lh_get_tail(&lh) == NULL);

	assert(lh_is_valid(NULL) == false);
	assert(lh_is_empty(NULL) == true);

	heap = lh_create(CM_LH_DOUBLY, 0);
	assert(heap != NULL);
	assert(lh_is_valid(heap) == true);
	lh_free(&heap);
	assert(heap == NULL);
	lh_free(&heap);
	lh_free(NULL);

	assert(lh_create((t_cm_lh_type)99, 0) == NULL);
	alloc_set_call_limit(0);
	assert(lh_create(CM_LH_SINGLY, 0) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	lh_destroy(&lh);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);
	lh_destroy(NULL);

	assert(lh_push(NULL, NULL) == NULL);
	assert(lh_push(&lh, NULL) == NULL);
	assert(lh_push_back(NULL, NULL) == NULL);
	assert(lh_push_back(&lh, NULL) == NULL);
	assert(lh_pop(NULL) == NULL);
	assert(lh_pop(&lh) == NULL);
	assert(lh_pop_back(NULL) == NULL);
	assert(lh_pop_back(&lh) == NULL);
	assert(lh_remove(NULL, NULL) == false);
	assert(lh_remove(&lh, NULL) == false);
	assert(lh_foreach(NULL, NULL, NULL) == NULL);
	assert(lh_foreach(&lh, NULL, NULL) == NULL);
	assert(lh_iter(NULL, NULL) == false);
	assert(lh_iter(NULL, &it) == false);
	assert(it == NULL);
	assert(lh_iter(&lh, NULL) == false);
	assert(lh_length(NULL) == 0);
	assert(lh_get_head(NULL) == NULL);
	assert(lh_get_tail(NULL) == NULL);
	lh_clear(NULL, NULL);
}

NO_COV void test_cm_lh_singly(void) {
	t_cm_test_node n[4];
	t_cm_lh        lh;
	t_cm_lh_state  st  = {0};
	void          *it  = NULL;
	size_t         off = offsetof(t_cm_test_node, node);
	size_t         i;

	for (i = 0; i < 4; i++) {
		INIT_NODE(n[i], (int)(i + 1), (int)i);
	}
	assert(lh_init(&lh, CM_LH_SINGLY, off) == &lh);

	for (i = 0; i < 4; i++) {
		assert(lh_push_back(&lh, &n[i]) == &n[0]);
		assert(lh_get_tail(&lh) == &n[i]);
		assert(lh_is_valid(&lh) == true);
	}
	assert(lh_length(&lh) == 4);
	assert(lh_get_head(&lh) == &n[0]);
	assert(lh_get_tail(&lh) == &n[3]);

	assert(lh_foreach(&lh, lh_record, &st) == &st);
	assert(st.count == 4);
	assert(st.values[0] == 1 && st.values[1] == 2);
	assert(st.values[2] == 3 && st.values[3] == 4);
	assert(lh_foreach(&lh, NULL, &st) == &st);

	st.count = 0;
	while (lh_iter(&lh, &it)) {
		st.values[st.count++] = ((t_cm_lh_prefix *)it)->value;
	}
	assert(st.count == 4);
	assert(it == NULL);

	assert(lh_pop(&lh) == &n[0]);
	assert(lh_get_head(&lh) == &n[1]);
	assert(lh_is_valid(&lh) == true);
	assert(lh_pop_back(&lh) == &n[3]);
	assert(lh_get_tail(&lh) == &n[2]);
	assert(lh_is_valid(&lh) == true);
	assert(lh_length(&lh) == 2);

	assert(lh_remove(&lh, &n[1]) == true);
	assert(lh_get_head(&lh) == &n[2]);
	assert(lh_get_tail(&lh) == &n[2]);
	assert(lh_length(&lh) == 1);
	assert(lh_remove(&lh, &n[1]) == false);

	g_dtor_hits = 0;
	lh_clear(&lh, cb_count_dtor);
	assert(g_dtor_hits == 1);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);

	assert(lh_push(&lh, &n[0]) == &n[0]);
	assert(lh_get_head(&lh) == &n[0]);
	assert(lh_get_tail(&lh) == &n[0]);
	assert(lh_push(&lh, &n[1]) == &n[1]);
	assert(lh_get_head(&lh) == &n[1]);
	assert(lh_get_tail(&lh) == &n[0]);
	assert(lh_is_valid(&lh) == true);

	lh_clear(&lh, NULL);
	assert(lh_is_empty(&lh) == true);
}

NO_COV void test_cm_lh_doubly(void) {
	t_cm_test_dnode n[4];
	t_cm_lh         lh;
	t_cm_lh_state   st  = {0};
	void           *it  = NULL;
	size_t          off = offsetof(t_cm_test_dnode, dnode);
	size_t          i;

	for (i = 0; i < 4; i++) {
		INIT_DNODE(n[i], (int)(i + 1), (int)i);
	}
	assert(lh_init(&lh, CM_LH_DOUBLY, off) == &lh);

	for (i = 0; i < 4; i++) {
		assert(lh_push_back(&lh, &n[i]) == &n[0]);
		assert(lh_get_tail(&lh) == &n[i]);
		assert(lh_is_valid(&lh) == true);
	}
	assert(lh_length(&lh) == 4);
	assert(lh_get_head(&lh) == &n[0]);
	assert(lh_get_tail(&lh) == &n[3]);

	assert(lh_foreach(&lh, lh_record, &st) == &st);
	assert(st.count == 4);
	assert(st.values[0] == 1 && st.values[3] == 4);

	st.count = 0;
	while (lh_iter(&lh, &it)) {
		st.values[st.count++] = ((t_cm_lh_prefix *)it)->value;
	}
	assert(st.count == 4);
	assert(it == NULL);

	assert(lh_pop(&lh) == &n[0]);
	assert(lh_get_head(&lh) == &n[1]);
	assert(lh_is_valid(&lh) == true);
	assert(lh_pop_back(&lh) == &n[3]);
	assert(lh_get_tail(&lh) == &n[2]);
	assert(lh_is_valid(&lh) == true);

	assert(lh_remove(&lh, &n[1]) == true);
	assert(lh_get_head(&lh) == &n[2]);
	assert(lh_get_tail(&lh) == &n[2]);
	assert(lh_length(&lh) == 1);
	assert(lh_remove(&lh, &n[1]) == false);

	g_dtor_hits = 0;
	lh_clear(&lh, cb_count_dtor);
	assert(g_dtor_hits == 1);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);

	assert(lh_push(&lh, &n[0]) == &n[0]);
	assert(lh_push(&lh, &n[1]) == &n[1]);
	assert(lh_get_head(&lh) == &n[1]);
	assert(lh_get_tail(&lh) == &n[0]);
	assert(lh_is_valid(&lh) == true);
	lh_clear(&lh, NULL);
}

NO_COV void test_cm_lh_circ_singly(void) {
	t_cm_test_node n[4];
	t_cm_lh        lh;
	t_cm_lh_state  st  = {0};
	void          *it  = NULL;
	size_t         off = offsetof(t_cm_test_node, node);
	size_t         i;

	for (i = 0; i < 4; i++) {
		INIT_NODE(n[i], (int)(i + 1), (int)i);
	}
	assert(lh_init(&lh, CM_LH_CIRC_SINGLY, off) == &lh);

	for (i = 0; i < 4; i++) {
		assert(lh_push_back(&lh, &n[i]) == &n[0]);
		assert(lh_get_tail(&lh) == &n[i]);
		assert(lh_is_valid(&lh) == true);
	}
	assert(lh_length(&lh) == 4);
	assert(lh_get_head(&lh) == &n[0]);
	assert(lh_get_tail(&lh) == &n[3]);

	assert(lh_foreach(&lh, lh_record, &st) == &st);
	assert(st.count == 4);
	assert(st.values[0] == 1 && st.values[1] == 2);
	assert(st.values[2] == 3 && st.values[3] == 4);
	assert(lh_foreach(&lh, NULL, &st) == &st);

	st.count = 0;
	while (lh_iter(&lh, &it)) {
		st.values[st.count++] = ((t_cm_lh_prefix *)it)->value;
	}
	assert(st.count == 4);
	assert(it == NULL);

	assert(lh_pop(&lh) == &n[0]);
	assert(lh_get_head(&lh) == &n[1]);
	assert(lh_is_valid(&lh) == true);
	assert(lh_pop_back(&lh) == &n[3]);
	assert(lh_get_tail(&lh) == &n[2]);
	assert(lh_is_valid(&lh) == true);
	assert(lh_length(&lh) == 2);

	assert(lh_remove(&lh, &n[1]) == true);
	assert(lh_get_head(&lh) == &n[2]);
	assert(lh_get_tail(&lh) == &n[2]);
	assert(lh_length(&lh) == 1);
	assert(lh_remove(&lh, &n[1]) == false);
	assert(lh_is_valid(&lh) == true);

	g_dtor_hits = 0;
	lh_clear(&lh, cb_count_dtor);
	assert(g_dtor_hits == 1);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);

	assert(lh_push(&lh, &n[0]) == &n[0]);
	assert(lh_push_back(&lh, &n[1]) == &n[0]);
	assert(lh_get_head(&lh) == &n[0]);
	assert(lh_get_tail(&lh) == &n[1]);
	assert(lh_is_valid(&lh) == true);
	assert(lh_pop(&lh) == &n[0]);
	assert(lh_pop_back(&lh) == &n[1]);
	assert(lh_is_empty(&lh) == true);
}

NO_COV void test_cm_lh_circ_doubly(void) {
	t_cm_test_dnode n[4];
	t_cm_lh         lh;
	t_cm_lh_state   st  = {0};
	void           *it  = NULL;
	size_t          off = offsetof(t_cm_test_dnode, dnode);
	size_t          i;

	for (i = 0; i < 4; i++) {
		INIT_DNODE(n[i], (int)(i + 1), (int)i);
	}
	assert(lh_init(&lh, CM_LH_CIRC_DOUBLY, off) == &lh);

	for (i = 0; i < 4; i++) {
		assert(lh_push_back(&lh, &n[i]) == &n[0]);
		assert(lh_get_tail(&lh) == &n[i]);
		assert(lh_is_valid(&lh) == true);
	}
	assert(lh_length(&lh) == 4);
	assert(lh_get_head(&lh) == &n[0]);
	assert(lh_get_tail(&lh) == &n[3]);

	assert(lh_foreach(&lh, lh_record, &st) == &st);
	assert(st.count == 4);
	assert(st.values[0] == 1 && st.values[3] == 4);

	st.count = 0;
	while (lh_iter(&lh, &it)) {
		st.values[st.count++] = ((t_cm_lh_prefix *)it)->value;
	}
	assert(st.count == 4);
	assert(it == NULL);

	assert(lh_pop(&lh) == &n[0]);
	assert(lh_get_head(&lh) == &n[1]);
	assert(lh_is_valid(&lh) == true);
	assert(lh_pop_back(&lh) == &n[3]);
	assert(lh_get_tail(&lh) == &n[2]);
	assert(lh_is_valid(&lh) == true);

	assert(lh_remove(&lh, &n[1]) == true);
	assert(lh_get_head(&lh) == &n[2]);
	assert(lh_get_tail(&lh) == &n[2]);
	assert(lh_length(&lh) == 1);
	assert(lh_remove(&lh, &n[1]) == false);

	g_dtor_hits = 0;
	lh_clear(&lh, cb_count_dtor);
	assert(g_dtor_hits == 1);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);

	assert(lh_push(&lh, &n[0]) == &n[0]);
	assert(lh_push_back(&lh, &n[1]) == &n[0]);
	assert(lh_get_head(&lh) == &n[0]);
	assert(lh_get_tail(&lh) == &n[1]);
	assert(lh_is_valid(&lh) == true);
	assert(lh_pop(&lh) == &n[0]);
	assert(lh_pop_back(&lh) == &n[1]);
	assert(lh_is_empty(&lh) == true);
}

NO_COV void test_cm_lh_invalid_type(void) {
	t_cm_test_node n;
	t_cm_lh        lh;
	t_cm_lh_state  st  = {0};
	void          *it  = NULL;
	size_t         off = offsetof(t_cm_test_node, node);

	INIT_NODE(n, 1, 1);
	lh_init(&lh, CM_LH_SINGLY, off);
	lh_push_back(&lh, &n);
	lh.type = (t_cm_lh_type)99;

	assert(lh_is_valid(&lh) == false);
	assert(lh_length(&lh) == 0);
	assert(lh_get_head(&lh) == NULL);
	assert(lh_get_tail(&lh) == NULL);
	assert(lh_push(&lh, &n) == NULL);
	assert(lh_push_back(&lh, &n) == NULL);
	assert(lh_pop(&lh) == NULL);
	assert(lh_pop_back(&lh) == NULL);
	assert(lh_remove(&lh, &n) == false);
	assert(lh_foreach(&lh, lh_record, &st) == &st);
	assert(lh_iter(&lh, &it) == false);
	lh_clear(&lh, NULL);
	assert(lh_is_empty(&lh) == false);
}

NO_COV void test_cm_lh_edges(void) {
	t_cm_test_node  s[2];
	t_cm_test_dnode d[2];
	t_cm_lh         lh;
	t_cm_lh         tmp;
	size_t          off_s = offsetof(t_cm_test_node, node);
	size_t          off_d = offsetof(t_cm_test_dnode, dnode);

	INIT_NODE(s[0], 1, 0);
	INIT_NODE(s[1], 2, 1);
	INIT_DNODE(d[0], 11, 0);
	INIT_DNODE(d[1], 12, 1);

	// singly: single-node pop_back, tail remove and empty concat
	lh_init(&lh, CM_LH_SINGLY, off_s);
	lh_push_back(&lh, &s[0]);
	assert(lh_pop(&lh) == &s[0]);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);
	lh_push_back(&lh, &s[0]);
	assert(lh_pop_back(&lh) == &s[0]);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);

	lh_push_back(&lh, &s[0]);
	lh_push_back(&lh, &s[1]);
	assert(lh_remove(&lh, &s[1]) == true);
	assert(lh_get_tail(&lh) == &s[0]);
	assert(lh_is_valid(&lh) == true);
	lh_clear(&lh, NULL);

	lh_init(&lh, CM_LH_SINGLY, off_s);
	lh_init(&tmp, CM_LH_SINGLY, off_s);
	lh_push_back(&lh, &s[0]);
	assert(lh_concat(&tmp, &lh) == true);
	assert(lh_is_empty(&lh) == true);
	assert(lh_length(&tmp) == 1);
	assert(lh_concat(&tmp, &lh) == true);
	assert(lh_length(&tmp) == 1);
	lh_clear(&tmp, NULL);

	// doubly: single-node pop_back, tail remove and empty concat
	lh_init(&lh, CM_LH_DOUBLY, off_d);
	lh_push_back(&lh, &d[0]);
	assert(lh_pop(&lh) == &d[0]);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);
	lh_push_back(&lh, &d[0]);
	assert(lh_pop_back(&lh) == &d[0]);
	assert(lh_is_empty(&lh) == true);
	assert(lh_is_valid(&lh) == true);

	lh_push_back(&lh, &d[0]);
	lh_push_back(&lh, &d[1]);
	assert(lh_remove(&lh, &d[1]) == true);
	assert(lh_get_tail(&lh) == &d[0]);
	assert(lh_is_valid(&lh) == true);
	lh_clear(&lh, NULL);

	lh_init(&lh, CM_LH_DOUBLY, off_d);
	lh_init(&tmp, CM_LH_DOUBLY, off_d);
	lh_push_back(&lh, &d[0]);
	assert(lh_concat(&tmp, &lh) == true);
	assert(lh_is_empty(&lh) == true);
	assert(lh_length(&tmp) == 1);
	assert(lh_concat(&tmp, &lh) == true);
	assert(lh_length(&tmp) == 1);
	lh_clear(&tmp, NULL);

	// circular singly: single-node pop_back and tail remove
	lh_init(&lh, CM_LH_CIRC_SINGLY, off_s);
	lh_push_back(&lh, &s[0]);
	assert(lh_pop_back(&lh) == &s[0]);
	assert(lh_is_empty(&lh) == true);
	lh_push_back(&lh, &s[0]);
	lh_push_back(&lh, &s[1]);
	assert(lh_remove(&lh, &s[1]) == true);
	assert(lh_get_tail(&lh) == &s[0]);
	assert(lh_is_valid(&lh) == true);
	lh_clear(&lh, NULL);

	// circular doubly: single-node pop_back and tail remove
	lh_init(&lh, CM_LH_CIRC_DOUBLY, off_d);
	lh_push_back(&lh, &d[0]);
	assert(lh_pop_back(&lh) == &d[0]);
	assert(lh_is_empty(&lh) == true);
	lh_push_back(&lh, &d[0]);
	lh_push_back(&lh, &d[1]);
	assert(lh_remove(&lh, &d[1]) == true);
	assert(lh_get_tail(&lh) == &d[0]);
	assert(lh_is_valid(&lh) == true);
	lh_clear(&lh, NULL);

	// removing a foreign element fails on every list type
	{
		t_cm_test_node  fs;
		t_cm_test_dnode fd;

		INIT_NODE(fs, 99, 9);
		INIT_DNODE(fd, 99, 9);

		lh_init(&lh, CM_LH_SINGLY, off_s);
		lh_push_back(&lh, &s[0]);
		assert(lh_remove(&lh, &fs) == false);
		lh_clear(&lh, NULL);

		lh_init(&lh, CM_LH_DOUBLY, off_d);
		lh_push_back(&lh, &d[0]);
		assert(lh_remove(&lh, &fd) == false);
		lh_clear(&lh, NULL);

		lh_init(&lh, CM_LH_CIRC_SINGLY, off_s);
		lh_push_back(&lh, &s[0]);
		assert(lh_remove(&lh, &fs) == false);
		lh_clear(&lh, NULL);

		lh_init(&lh, CM_LH_CIRC_DOUBLY, off_d);
		lh_push_back(&lh, &d[0]);
		assert(lh_remove(&lh, &fd) == false);
		lh_clear(&lh, NULL);
	}

	// empty handlers reject non-null removals
	INIT_NODE(s[0], 1, 0);
	INIT_DNODE(d[0], 11, 0);
	lh_init(&lh, CM_LH_SINGLY, off_s);
	assert(lh_remove(&lh, &s[0]) == false);

	// circular lists can be emptied through pop, pop_back and remove
	INIT_NODE(s[0], 1, 0);
	lh_init(&lh, CM_LH_CIRC_SINGLY, off_s);
	lh_push_back(&lh, &s[0]);
	assert(lh_pop(&lh) == &s[0]);
	assert(lh_is_empty(&lh) == true);
	INIT_NODE(s[0], 1, 0);
	lh_push_back(&lh, &s[0]);
	assert(lh_pop_back(&lh) == &s[0]);
	assert(lh_is_empty(&lh) == true);
	INIT_NODE(s[0], 1, 0);
	lh_push_back(&lh, &s[0]);
	assert(lh_remove(&lh, &s[0]) == true);
	assert(lh_is_empty(&lh) == true);

	INIT_DNODE(d[0], 11, 0);
	lh_init(&lh, CM_LH_CIRC_DOUBLY, off_d);
	lh_push_back(&lh, &d[0]);
	assert(lh_pop(&lh) == &d[0]);
	assert(lh_is_empty(&lh) == true);
	INIT_DNODE(d[0], 11, 0);
	lh_push_back(&lh, &d[0]);
	assert(lh_pop_back(&lh) == &d[0]);
	assert(lh_is_empty(&lh) == true);
	INIT_DNODE(d[0], 11, 0);
	lh_push_back(&lh, &d[0]);
	assert(lh_remove(&lh, &d[0]) == true);
	assert(lh_is_empty(&lh) == true);

	// inconsistent cached pointers are rejected by the validity check
	INIT_NODE(s[0], 1, 0);
	INIT_DNODE(d[0], 11, 0);
	lh_init(&lh, CM_LH_SINGLY, off_s);
	lh_push_back(&lh, &s[0]);
	lh.head = NULL;
	assert(lh_is_valid(&lh) == false);
	lh.head = &s[0].node;
	lh.tail = NULL;
	assert(lh_is_valid(&lh) == false);

	INIT_DNODE(d[0], 11, 0);
	lh_init(&lh, CM_LH_DOUBLY, off_d);
	lh_push_back(&lh, &d[0]);
	((t_cm_idl *)lh.head)->prev = lh.tail;
	assert(lh_is_valid(&lh) == false);
	((t_cm_idl *)lh.head)->prev = NULL;
	((t_cm_idl *)lh.tail)->next = lh.head;
	assert(lh_is_valid(&lh) == false);

	INIT_DNODE(d[0], 11, 0);
	INIT_DNODE(d[1], 12, 1);
	lh_init(&lh, CM_LH_CIRC_DOUBLY, off_d);
	lh_push_back(&lh, &d[0]);
	lh_push_back(&lh, &d[1]);
	((t_cm_idl *)lh.head)->prev = lh.head;
	assert(lh_is_valid(&lh) == false);
	((t_cm_idl *)lh.head)->prev = lh.tail;
	((t_cm_idl *)lh.tail)->next = lh.tail;
	assert(lh_is_valid(&lh) == false);

	// concatenating handlers with an invalid type reports failure
	lh_init(&lh, CM_LH_SINGLY, off_s);
	lh_init(&tmp, CM_LH_SINGLY, off_s);
	lh_push_back(&lh, &s[0]);
	lh_push_back(&tmp, &s[1]);
	lh.type  = (t_cm_lh_type)99;
	tmp.type = (t_cm_lh_type)99;
	assert(lh_concat(&lh, &tmp) == false);
}

NO_COV void test_cm_lh_concat(void) {
	t_cm_test_node  a[2], b[2];
	t_cm_test_dnode da[2], db[2];
	t_cm_lh         l1, l2, l3;
	t_cm_lh_state   st    = {0};
	size_t          off_s = offsetof(t_cm_test_node, node);
	size_t          off_d = offsetof(t_cm_test_dnode, dnode);
	size_t          i;

	for (i = 0; i < 2; i++) {
		INIT_NODE(a[i], (int)(i + 1), (int)i);
		INIT_NODE(b[i], (int)(i + 3), (int)(i + 2));
		INIT_DNODE(da[i], (int)(i + 1), (int)i);
		INIT_DNODE(db[i], (int)(i + 3), (int)(i + 2));
	}

	// singly
	lh_init(&l1, CM_LH_SINGLY, off_s);
	lh_init(&l2, CM_LH_SINGLY, off_s);
	lh_init(&l3, CM_LH_DOUBLY, off_d);
	for (i = 0; i < 2; i++) {
		lh_push_back(&l1, &a[i]);
		lh_push_back(&l2, &b[i]);
	}
	assert(lh_concat(&l1, &l1) == false);
	assert(lh_concat(NULL, &l1) == false);
	assert(lh_concat(&l1, NULL) == false);
	assert(lh_concat(&l1, &l3) == false);
	lh_init(&l3, CM_LH_SINGLY, off_s + 1);
	assert(lh_concat(&l1, &l3) == false);
	assert(lh_concat(&l1, &l2) == true);
	assert(lh_length(&l1) == 4);
	assert(lh_get_head(&l1) == &a[0]);
	assert(lh_get_tail(&l1) == &b[1]);
	assert(lh_is_empty(&l2) == true);
	assert(lh_is_valid(&l1) == true);
	assert(lh_foreach(&l1, lh_record, &st) == &st);
	assert(st.count == 4);
	assert(st.values[0] == 1 && st.values[1] == 2);
	assert(st.values[2] == 3 && st.values[3] == 4);
	assert(lh_foreach(&l1, NULL, &st) == &st);

	assert(lh_concat(&l1, &l2) == true);
	assert(lh_length(&l1) == 4);
	lh_init(&l3, CM_LH_SINGLY, off_s);
	assert(lh_concat(&l3, &l1) == true);
	assert(lh_length(&l3) == 4);
	assert(lh_is_empty(&l1) == true);
	assert(lh_is_valid(&l3) == true);

	// doubly
	lh_init(&l1, CM_LH_DOUBLY, off_d);
	lh_init(&l2, CM_LH_DOUBLY, off_d);
	for (i = 0; i < 2; i++) {
		lh_push_back(&l1, &da[i]);
		lh_push_back(&l2, &db[i]);
	}
	assert(lh_concat(&l1, &l2) == true);
	assert(lh_length(&l1) == 4);
	assert(lh_get_head(&l1) == &da[0]);
	assert(lh_get_tail(&l1) == &db[1]);
	assert(lh_is_empty(&l2) == true);
	assert(lh_is_valid(&l1) == true);

	// circular singly
	lh_init(&l1, CM_LH_CIRC_SINGLY, off_s);
	lh_init(&l2, CM_LH_CIRC_SINGLY, off_s);
	for (i = 0; i < 2; i++) {
		lh_push_back(&l1, &a[i]);
		lh_push_back(&l2, &b[i]);
	}
	assert(lh_concat(&l1, &l2) == true);
	assert(lh_length(&l1) == 4);
	assert(lh_get_head(&l1) == &a[0]);
	assert(lh_get_tail(&l1) == &b[1]);
	assert(lh_is_empty(&l2) == true);
	assert(lh_is_valid(&l1) == true);
	st.count = 0;
	assert(lh_foreach(&l1, lh_record, &st) == &st);
	assert(st.count == 4);
	assert(st.values[0] == 1 && st.values[1] == 2);
	assert(st.values[2] == 3 && st.values[3] == 4);
	assert(lh_foreach(&l1, NULL, &st) == &st);

	// circular doubly
	lh_init(&l1, CM_LH_CIRC_DOUBLY, off_d);
	lh_init(&l2, CM_LH_CIRC_DOUBLY, off_d);
	for (i = 0; i < 2; i++) {
		lh_push_back(&l1, &da[i]);
		lh_push_back(&l2, &db[i]);
	}
	assert(lh_concat(&l1, &l2) == true);
	assert(lh_length(&l1) == 4);
	assert(lh_get_head(&l1) == &da[0]);
	assert(lh_get_tail(&l1) == &db[1]);
	assert(lh_is_empty(&l2) == true);
	assert(lh_is_valid(&l1) == true);
}
