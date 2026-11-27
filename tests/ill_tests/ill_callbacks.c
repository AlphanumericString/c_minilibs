#include <stdlib.h>

#include "ill_tests.h"

// general
NO_COV bool cb_true(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return true;
}
NO_COV bool cb_false(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return false;
}
NO_COV void *cb_bad_duplicator(const void *elem) {
	(void)elem;
	return NULL;
}
NO_COV void *cb_ill_sum(void *elem, void *arg) {
	t_cm_test_node *n   = (t_cm_test_node *)elem;
	int            *acc = (int *)arg;

	*acc += n->value;
	return (arg);
}
NO_COV void *cb_idl_sum(void *elem, void *arg) {
	t_cm_test_dnode *n   = (t_cm_test_dnode *)elem;
	int             *acc = (int *)arg;

	*acc += n->value;
	return (arg);
}

int g_dtor_hits   = 0;
int g_dup_hits    = 0;
int g_dup_fail_at = -1;

NO_COV void cb_free_elem(void *elem) {
	free(elem);
}
NO_COV void cb_count_dtor(void *elem) {
	(void)elem;
	g_dtor_hits++;
}
NO_COV void *cb_ill_duplicate_fail_at(const void *elem) {
	if (g_dup_fail_at >= 0 && g_dup_hits >= g_dup_fail_at) return NULL;
	g_dup_hits++;
	return cb_ill_duplicate(elem);
}
NO_COV void *cb_idl_duplicate_fail_at(const void *elem) {
	if (g_dup_fail_at >= 0 && g_dup_hits >= g_dup_fail_at) return NULL;
	g_dup_hits++;
	return cb_idl_duplicate(elem);
}

// idl only
NO_COV bool cb_idl_add_val_as_int_ptr(void *elem, void *arg) {
	t_cm_test_dnode *n   = (t_cm_test_dnode *)elem;
	int             *val = (int *)arg;

	n->value += *val;
	return true;
}
NO_COV bool cb_idl_it_add_pos(void *elem, void *arg, size_t pos, bool last) {
	t_cm_test_dnode *n = (t_cm_test_dnode *)elem;

	(void)arg;
	(void)last;
	n->value += (int)pos;
	return true;
}
NO_COV bool cb_idl_it_record(void *elem, void *arg, size_t pos, bool last) {
	t_cm_test_dnode *n = (t_cm_test_dnode *)elem;
	t_cm_it_state   *s = (t_cm_it_state *)arg;

	if (!s->count) s->first_pos = pos;
	s->count++;
	s->last_pos = pos;
	if (last) s->nb_last++;
	s->sum += n->value;
	return true;
}
NO_COV bool cb_idl_it_stop(void *elem, void *arg, size_t pos, bool last) {
	t_cm_test_dnode *n = (t_cm_test_dnode *)elem;

	(void)last;
	n->value += 100;
	return pos < *(size_t *)arg;
}
NO_COV bool cb_idl_false_on_arg(void *elem, void *arg) {
	t_cm_test_dnode *n = (t_cm_test_dnode *)elem;
	int              val;

	if (!arg) return (false);
	val = *(int *)arg;
	return n->value <= val;
}
NO_COV bool cb_idl_has_val_42(void *elem, void *arg) {
	t_cm_test_dnode *n = (t_cm_test_dnode *)elem;

	(void)arg;
	return n->value == 42;
}
NO_COV bool cb_idl_is_even(void *elem, void *arg) {
	t_cm_test_dnode *n = (t_cm_test_dnode *)elem;

	(void)arg;
	return n->value % 2 == 0;
}
NO_COV int cb_idl_cmp_val(const void *elem, const void *arg) {
	t_cm_test_dnode *n   = (t_cm_test_dnode *)elem;
	int              val = *(int *)arg;

	return n->value - val;
}
NO_COV int cb_idl_cmp_min_max(const void *elem1, const void *elem2) {
	t_cm_test_dnode *n1 = (t_cm_test_dnode *)elem1;
	t_cm_test_dnode *n2 = (t_cm_test_dnode *)elem2;

	return n1->value - n2->value;
}
NO_COV void *cb_idl_duplicate(const void *elem) {
	t_cm_test_dnode *src  = (t_cm_test_dnode *)elem;
	t_cm_test_dnode *copy = malloc(sizeof(t_cm_test_dnode));

	if (copy) {
		copy->id    = src->id;
		copy->value = src->value;
		idl_init_node(&copy->dnode, offsetof(t_cm_test_dnode, dnode));
	}
	return copy;
}

// ill only
NO_COV bool cb_ill_add_val_as_int_ptr(void *elem, void *arg) {
	t_cm_test_node *n   = (t_cm_test_node *)elem;
	int            *val = (int *)arg;

	n->value += *val;
	return true;
}
NO_COV bool cb_ill_it_add_pos(void *elem, void *arg, size_t pos, bool last) {
	t_cm_test_node *n = (t_cm_test_node *)elem;

	(void)arg;
	(void)last;
	n->value += (int)pos;
	return true;
}
NO_COV bool cb_ill_it_record(void *elem, void *arg, size_t pos, bool last) {
	t_cm_test_node *n = (t_cm_test_node *)elem;
	t_cm_it_state  *s = (t_cm_it_state *)arg;

	if (!s->count) s->first_pos = pos;
	s->count++;
	s->last_pos = pos;
	if (last) s->nb_last++;
	s->sum += n->value;
	return true;
}
NO_COV bool cb_ill_it_stop(void *elem, void *arg, size_t pos, bool last) {
	t_cm_test_node *n = (t_cm_test_node *)elem;

	(void)last;
	n->value += 100;
	return pos < *(size_t *)arg;
}
NO_COV bool cb_ill_stop_at_30(void *elem, void *arg) {
	t_cm_test_node *n = (t_cm_test_node *)elem;

	(void)arg;
	return n->value < 30;
}
NO_COV bool cb_ill_has_val_42(void *elem, void *arg) {
	t_cm_test_node *n = (t_cm_test_node *)elem;

	(void)arg;
	return n->value == 42;
}
NO_COV bool cb_ill_is_even(void *elem, void *arg) {
	t_cm_test_node *n = (t_cm_test_node *)elem;

	(void)arg;
	return n->value % 2 == 0;
}
NO_COV int cb_ill_cmp_val(const void *elem, const void *arg) {
	t_cm_test_node *n   = (t_cm_test_node *)elem;
	int             val = *(int *)arg;

	return n->value - val;
}
NO_COV int cb_ill_cmp_min_max(const void *elem1, const void *elem2) {
	t_cm_test_node *n1 = (t_cm_test_node *)elem1;
	t_cm_test_node *n2 = (t_cm_test_node *)elem2;

	return n1->value - n2->value;
}
NO_COV void *cb_ill_duplicate(const void *elem) {
	t_cm_test_node *src  = (t_cm_test_node *)elem;
	t_cm_test_node *copy = malloc(sizeof(t_cm_test_node));

	if (copy) {
		copy->id    = src->id;
		copy->value = src->value;
		ill_init_node(&copy->node, offsetof(t_cm_test_node, node));
	}
	return copy;
}
