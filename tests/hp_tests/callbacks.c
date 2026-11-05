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
