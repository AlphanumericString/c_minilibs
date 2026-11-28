#include <stdio.h>
#include <stdlib.h>

#include "rb_tests.h"

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

NO_COV bool cb_dump_int(void *elem, void *arg) {
	(void)arg;
	printf("%d ", *(int *)elem);
	return true;
}
NO_COV bool cb_add_one(void *elem, void *arg) {
	(void)arg;
	*(int *)elem += 1;
	return true;
}

NO_COV bool cb_it_collect(void *elem, void *arg, size_t pos, bool last) {
	t_rb_it_ctx *ctx = arg;

	if (!ctx->count) ctx->first_pos = pos;
	ctx->count++;
	ctx->sum     += *(int *)elem;
	ctx->last_pos = pos;
	if (last) ctx->saw_last = true;
	return (true);
}

NO_COV bool cb_it_stop(void *elem, void *arg, size_t pos, bool last) {
	t_rb_it_ctx *ctx = arg;

	(void)pos;
	(void)last;
	ctx->count++;
	ctx->sum += *(int *)elem;
	return (ctx->count < ctx->stop_at);
}

NO_COV int cb_cmp(const void *a, const void *b) {
	return *(int *)a - *(int *)b;
}

NO_COV void *cb_dup_fail3(const void *elem) {
	int *res;

	if (*(int *)elem == 3) return (NULL);
	res = malloc(sizeof(int));
	if (res) *res = *(const int *)elem;
	return res;
}
NO_COV void *cb_dup(const void *elem) {
	int *res = malloc(sizeof(int));
	if (res) *res = *(const int *)elem;
	return res;
}

NO_COV void cb_destroy(void *elem) {
	(void)elem;
}
