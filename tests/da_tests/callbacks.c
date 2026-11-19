#include "da_tests.h"

// Helper callbacks
NO_COV bool cb_true(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return true;
}
NO_COV bool cb_dump_int(void *e, void *a) {
	(void)a;
	return (printf("%d\n", *(int *)e), true);
}
NO_COV bool cb_filter_strict_inf(void *e, void *a) {
	int _e = *(int *)e;
	int _a = *(int *)a;

	return (_e) < (_a);
}
NO_COV bool cb_add_one(void *elem, void *arg) {
	(void)elem;
	(void)arg;

	*(int *)elem += 1;
	return true;
}

NO_COV bool cb_false(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return false;
}
NO_COV bool cb_it_true(void *elem, void *arg, size_t pos, bool last) {
	(void)elem;
	(void)arg;
	(void)pos;
	(void)last;
	return true;
}
NO_COV bool cb_it_add_it_to_arg(void *elem, void *arg, size_t pos, bool last) {
	(void)last;

	if ((unsigned long)arg <= pos) return (false);
	*(int *)elem += (int)pos;
	return (true);
}

NO_COV bool cb_it_record(void *elem, void *arg, size_t pos, bool last) {
	t_da_range_state *s = (t_da_range_state *)arg;

	if (!s->count) s->first_pos = pos;
	s->count++;
	s->last_pos = pos;
	if (last) s->nb_last++;
	s->sum += *(int *)elem;
	return (true);
}

NO_COV bool cb_it_stop(void *elem, void *arg, size_t pos, bool last) {
	(void)last;
	*(int *)elem += 100;
	return (pos < *(size_t *)arg);
}

NO_COV bool cb_empty_deletor(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return (true);
}
NO_COV void *cb_merger(void *elem, void *arg) {
	*(int *)arg += *(int *)elem;
	return arg;
}
NO_COV int cb_cmp(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

NO_COV void *cb_map_double(const void *elem) {
	static int out;

	out = *(const int *)elem * 2;
	return (&out);
}

NO_COV void *cb_map_to_long(const void *elem) {
	static long out;

	out = (long)*(const int *)elem * 10L;
	return (&out);
}

NO_COV void *cb_map_fail_on_3(const void *elem) {
	static int out;

	if (*(const int *)elem == 3) return (NULL);
	out = *(const int *)elem;
	return (&out);
}

NO_COV void *cb_map_null(const void *elem) {
	(void)elem;
	return (NULL);
}
