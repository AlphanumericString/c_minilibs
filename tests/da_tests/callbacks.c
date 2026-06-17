#include "da_tests.h"

// Helper callbacks
bool cb_true(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return true;
}
bool cb_dump_int(void *e, void *a) {
	(void)a;
	return (printf("%d\n", *(int *)e), true);
}
bool cb_filter_strict_inf(void *e, void *a) {
	int _e = *(int *)e;
	int _a = *(int *)a;

	return (_e) < (_a);
}
bool cb_add_one(void *elem, void *arg) {
	(void)elem;
	(void)arg;

	*(int *)elem += 1;
	return true;
}

bool cb_false(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return false;
}
bool cb_it_true(void *elem, void *arg, size_t pos, bool last) {
	(void)elem;
	(void)arg;
	(void)pos;
	(void)last;
	return true;
}
bool cb_it_add_it_to_arg(void *elem, void *arg, size_t pos, bool last) {
	(void)last;

	if ((unsigned long)arg <= pos) return (false);
	*(int *)elem += (int)pos;
	return (true);
}

bool cb_empty_deletor(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return (true);
}
void *cb_merger(void *elem, void *arg) {
	*(int *)arg += *(int *)elem;
	return arg;
}
int cb_cmp(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}
