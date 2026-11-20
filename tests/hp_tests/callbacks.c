#include "hp_tests.h"

NO_COV int cb_cmp_int(const void *a, const void *b) {
	return (*(const int *)a - *(const int *)b);
}
NO_COV bool cb_true(void *e, void *a) {
	(void)e;
	(void)a;
	return (true);
}
NO_COV bool cb_false(void *e, void *a) {
	(void)e;
	(void)a;
	return (false);
}
NO_COV bool cb_fail_at(void *e, void *a) {
	return (*(int *)e != *(int *)a);
}
NO_COV bool cb_log_int(void *e, void *a) {
	t_cm_order_ctx *ctx = (t_cm_order_ctx *)a;

	if (ctx->n < HP_ORDER_MAX) ctx->vals[ctx->n++] = *(int *)e;
	return (true);
}
NO_COV bool cb_it_log(void *e, void *a, size_t pos, bool last) {
	t_cm_order_ctx *ctx = (t_cm_order_ctx *)a;

	if (ctx->n < HP_ORDER_MAX) {
		ctx->vals[ctx->n] = *(int *)e;
		ctx->pos[ctx->n]  = pos;
		ctx->last[ctx->n] = last;
		ctx->n++;
	}
	return (true);
}
NO_COV bool cb_it_false(void *e, void *a, size_t pos, bool last) {
	(void)e;
	(void)a;
	(void)pos;
	(void)last;
	return (false);
}
NO_COV bool cb_keep_even(void *e, void *a) {
	(void)a;
	return ((*(int *)e % 2) == 0);
}
NO_COV bool cb_keep_all(void *e, void *a) {
	(void)e;
	(void)a;
	return (true);
}
NO_COV bool cb_keep_none(void *e, void *a) {
	(void)e;
	(void)a;
	return (false);
}
NO_COV bool cb_del_count(void *e, void *a) {
	(void)e;
	(*(size_t *)a)++;
	return (true);
}
NO_COV void *cb_merger(void *elem, void *arg) {
	*(int *)arg += *(int *)elem;
	return (arg);
}
NO_COV void *cb_map_identity(const void *elem) {
	return ((void *)elem);
}
NO_COV void *cb_map_fail(const void *elem) {
	(void)elem;
	return (NULL);
}
