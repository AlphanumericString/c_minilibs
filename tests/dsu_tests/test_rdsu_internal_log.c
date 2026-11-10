#include <assert.h>

#define C_MINI__RDSU_INTERNAL
#include "rdsu_tests.h"

NO_COV void test_cm_rdsui_log_stack(void) {
	t_cm_rdsu      rdsu;
	t_cm_rdsu_log *a;
	t_cm_rdsu_log *b;
	t_cm_rdsu_log *c;

	rdsu_init(&rdsu, 5);

	// popping empty stacks returns null and keeps the counter untouched
	assert(rdsui_log_pop_rewind(&rdsu) == NULL);
	assert(rdsui_log_pop_reserved(&rdsu) == NULL);
	assert(rdsui_log_pop_rewind(NULL) == NULL);
	assert(rdsui_log_pop_reserved(NULL) == NULL);
	assert(rdsu_reserved_logs_nb(&rdsu) == 0);

	// get fails when the reserve is empty and allocation is forbidden
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(rdsui_log_get(&rdsu) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(rdsu_reserved_logs_nb(&rdsu) == 0);

	// get allocates when the reserve is empty
	alloc_reset_counter();
	a = rdsui_log_get(&rdsu);
	assert(a != NULL);
	assert(nb_alloc == 1);
	assert(rdsu_reserved_logs_nb(&rdsu) == 0);

	// rewind stack push/pop is LIFO and keeps links clean
	rdsui_log_push_rewind(&rdsu, a);
	assert(rdsu.change_log == a);
	assert(a->nxt == NULL);
	assert(rdsui_log_pop_rewind(&rdsu) == a);
	assert(rdsu.change_log == NULL);
	assert(a->nxt == NULL);

	// reserved stack push/pop maintains the counter
	rdsui_log_push_reserved(&rdsu, a);
	assert(rdsu.reserved_logs == a);
	assert(rdsu_reserved_logs_nb(&rdsu) == 1);
	assert(rdsui_log_pop_reserved(&rdsu) == a);
	assert(rdsu.reserved_logs == NULL);
	assert(rdsu_reserved_logs_nb(&rdsu) == 0);

	// get reuses the reserve before allocating
	rdsui_log_push_reserved(&rdsu, a);
	alloc_reset_counter();
	alloc_set_call_limit(0);
	b = rdsui_log_get(&rdsu);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(b == a);
	assert(nb_alloc == 0);
	assert(rdsu_reserved_logs_nb(&rdsu) == 0);

	// several reserved logs pop in LIFO order
	c = rdsui_log_get(&rdsu);
	assert(c != NULL);
	rdsui_log_push_reserved(&rdsu, b);
	rdsui_log_push_reserved(&rdsu, c);
	assert(rdsu_reserved_logs_nb(&rdsu) == 2);
	assert(rdsui_log_pop_reserved(&rdsu) == c);
	assert(rdsui_log_pop_reserved(&rdsu) == b);
	assert(rdsu_reserved_logs_nb(&rdsu) == 0);

	// clear frees both stacks and resets the counter
	rdsui_log_push_reserved(&rdsu, b);
	rdsui_log_push_rewind(&rdsu, c);
	assert(rdsu_reserved_logs_nb(&rdsu) == 1);
	rdsui_log_clear(&rdsu);
	assert(rdsu.change_log == NULL);
	assert(rdsu.reserved_logs == NULL);
	assert(rdsu_reserved_logs_nb(&rdsu) == 0);
	rdsui_log_clear(NULL);

	assert(rdsui_log_get(NULL) == NULL);

	rdsu_destroy(&rdsu);
}
