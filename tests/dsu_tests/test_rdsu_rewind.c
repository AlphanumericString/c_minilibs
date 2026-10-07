#include <assert.h>

#define C_MINI__RDSU_INTERNAL
#include "rdsu_tests.h"

void test_rdsui_push_rewind(void) {
	t_rdsu rdsu;

	rdsu_init(&rdsu, 10);
	assert(rdsui_push_rewind(&rdsu, 0, 0, 0, RDSU_LOG_RANK) == true);
	assert(rdsui_push_rewind(&rdsu, 0, -1, 0, RDSU_LOG_RANK) == false);
	assert(rdsui_push_rewind(&rdsu, 0, -1, 0, RDSU_LOG_PARENT) == false);
	assert(rdsui_push_rewind(&rdsu, 0, 0, -1, RDSU_LOG_RANK) == false);
	assert(rdsui_push_rewind(&rdsu, 0, 0, -1, RDSU_LOG_PARENT) == true);
	assert(rdsui_push_rewind(&rdsu, 0, 0, 0, 999) == false);
	rdsu_destroy(&rdsu);
	return;
}
void test_rdsu_save_state(void) {
	t_rdsu rdsu;

	rdsu_init(&rdsu, 10);
	assert(rdsu_save_state(&rdsu) == NULL);
	rdsu_union(&rdsu, 1, 2);
	assert(rdsu_save_state(&rdsu) != NULL);
	assert(rdsu_save_state(NULL) == NULL);
	rdsu_destroy(&rdsu);
	assert(rdsu_save_state(&rdsu) == NULL);
	return;
}
void test_rdsu_rewind(void) {
	t_rdsu     rdsu;
	t_rdsu     intact_copy;
	t_rdsu_log malicious_log;

	rdsu_init(&rdsu, 5);
	rdsu_union(&rdsu, 1, 2);
	rdsu_union(&rdsu, 0, 2);                            // 0-1-2 3 4
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 2)); // state1 assertion
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 3));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 4));
	assert(rdsu_find(&rdsu, 3) != rdsu_find(&rdsu, 4));
	assert(rdsu_nb_components(rdsu) == 3);
	rdsu_union(&rdsu, 3, 4);
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 2)); // state2
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 3));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 4));
	assert(rdsu_find(&rdsu, 3) == rdsu_find(&rdsu, 4));
	assert(rdsu_nb_components(rdsu) == 2);
	rdsu_rewind(&rdsu);
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 2)); // state1
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 3));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 4));
	assert(rdsu_find(&rdsu, 3) != rdsu_find(&rdsu, 4));
	assert(rdsu_nb_components(rdsu) == 3);
	intact_copy = rdsu;

	rdsu.parent = NULL;
	assert(rdsu_rewind(&rdsu) == false);
	rdsu      = intact_copy;
	rdsu.rank = NULL;
	assert(rdsu_rewind(&rdsu) == false);
	rdsu      = intact_copy;
	rdsu.size = 0;
	assert(rdsu_rewind(&rdsu) == false);
	rdsu = intact_copy;

	malicious_log = (t_rdsu_log){
	    .elem = 0,
	    .nxt  = rdsu.change_log,
	    .p    = 0,
	    .r    = 0,
	    .type = 999,
	};
	rdsu.change_log = &malicious_log;
	assert(malicious_log.type != RDSU_LOG_RANK &&
	       malicious_log.type != RDSU_LOG_PARENT);
	assert(rdsu_rewind(&rdsu) == false);
	malicious_log.type = RDSU_LOG_RANK;
	malicious_log.p    = (uint16_t)-1;
	assert(rdsu_rewind(&rdsu) == false);
	malicious_log.p = 0;
	malicious_log.r = (uint16_t)-1;
	assert(rdsu_rewind(&rdsu) == false);
	malicious_log.r    = 0;
	malicious_log.elem = (uint16_t)-1;
	assert(rdsu_rewind(&rdsu) == false);
	malicious_log.elem = 0;

	rdsu.change_log = rdsu.change_log->nxt;
	rdsu_destroy(&rdsu);
	assert(rdsu_rewind(&rdsu) == false);
	assert(rdsu_rewind(NULL) == false);
	return;
}
void test_rdsu_rewind_to_state(void) {
	t_rdsu_log *log;
	t_rdsu      rdsu;
	t_rdsu_log  malicious_log;

	rdsu_init(&rdsu, 5);
	rdsu_union(&rdsu, 1, 2); // 0 1-2 3 4
	log = rdsu_save_state(&rdsu);
	assert(log != NULL);
	assert(rdsu_find(&rdsu, 0) == 0); // assert state 1
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 3) == 3);
	assert(rdsu_find(&rdsu, 4) == 4);
	assert(rdsu_nb_components(rdsu) == 4);
	rdsu_union(&rdsu, 0, 2); // 0-1-2 3 4
	rdsu_union(&rdsu, 3, 4); // 0-1-2 3-4
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 1));
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 3) == rdsu_find(&rdsu, 4));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 3));
	assert(rdsu_nb_components(rdsu) == 2);
	assert(rdsu_rewind_to_state(&rdsu, log) == true);
	assert(log != NULL); // assert state 1
	assert(rdsu_find(&rdsu, 0) == 0);
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 3) == 3);
	assert(rdsu_find(&rdsu, 4) == 4);
	assert(rdsu_nb_components(rdsu) == 4);

	malicious_log = (t_rdsu_log){
	    .elem = 0,
	    .nxt  = rdsu.change_log,
	    .p    = 0,
	    .r    = 0,
	    .type = 999,
	};

	assert(rdsu_rewind_to_state(&rdsu, NULL) == false);
	assert(rdsu_rewind_to_state(NULL, log) == false);
	rdsu.change_log = &malicious_log;
	assert(malicious_log.type != RDSU_LOG_RANK &&
	       malicious_log.type != RDSU_LOG_PARENT);
	// log w bad type as input and in change_log
	assert(rdsu_rewind_to_state(&rdsu, &malicious_log) == false);
	// log w good type in change_log and bad log in change_log
	assert(rdsu_rewind_to_state(&rdsu, log) == false);
	assert(rdsu.change_log == &malicious_log);
	rdsu.change_log = rdsu.change_log->nxt;
	// log w bad type as input and good in change_log
	assert(rdsu_rewind_to_state(&rdsu, &malicious_log) == false);
	log->nxt        = NULL;
	rdsu.change_log = log;
	malicious_log   = *log;
	// log w good type as input and good in change_log but not in change_log
	// list
	assert(rdsu_rewind_to_state(&rdsu, &malicious_log) == false);
	rdsu_clr(&rdsu);
	// any log with empty change_log list
	assert(rdsu_rewind_to_state(&rdsu, (void *)0xDEAD) == false);

	rdsu_destroy(&rdsu);
	return;
}
void test_rdsu_rewind_clear(void) {
	t_rdsu rdsu;

	rdsu_init(&rdsu, 10);
	assert(rdsu.change_log == NULL);
	rdsu_union(&rdsu, 1, 2);
	rdsu_union(&rdsu, 0, 2); // 0-1-2 3 4
	rdsu_union(&rdsu, 3, 4); // 0-1-2 3-4
	assert(rdsu.change_log != NULL);
	rdsu_rewind_clear(&rdsu);
	assert(rdsu.change_log == NULL);
	assert(rdsu_rewind(&rdsu) == false);
	rdsu_rewind_clear(NULL);
	rdsu_destroy(&rdsu);
	return;
}
