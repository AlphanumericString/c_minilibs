#include <assert.h>

#define C_MINI__RDSU_INTERNAL
#include "rdsu_tests.h"

#define RDSU_BAD____TYPE 999

NO_COV void test_rdsu_save_state(void) {
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
NO_COV void test_rdsu_rewind(void) {
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
	assert(rdsu_nb_components(&rdsu) == 3);
	rdsu_union(&rdsu, 3, 4);
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 2)); // state2
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 3));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 4));
	assert(rdsu_find(&rdsu, 3) == rdsu_find(&rdsu, 4));
	assert(rdsu_nb_components(&rdsu) == 2);
	rdsu_rewind(&rdsu);
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 2)); // state1
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 3));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 4));
	assert(rdsu_find(&rdsu, 3) != rdsu_find(&rdsu, 4));
	assert(rdsu_nb_components(&rdsu) == 3);
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
	assert(malicious_log.type != RDSU_LOG_YESRANK &&
	       malicious_log.type != RDSU_LOG_NAYRANK);
	assert(rdsu_rewind(&rdsu) == false);
	malicious_log.type = RDSU_LOG_YESRANK;
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
NO_COV void test_rdsu_rewind_to_state(void) {
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
	assert(rdsu_nb_components(&rdsu) == 4);
	rdsu_union(&rdsu, 0, 2); // 0-1-2 3 4
	rdsu_union(&rdsu, 3, 4); // 0-1-2 3-4
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 1));
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 3) == rdsu_find(&rdsu, 4));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 3));
	assert(rdsu_nb_components(&rdsu) == 2);
	assert(rdsu_rewind_to_state(&rdsu, log) == true);
	assert(log != NULL); // assert state 1
	assert(rdsu_find(&rdsu, 0) == 0);
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 3) == 3);
	assert(rdsu_find(&rdsu, 4) == 4);
	assert(rdsu_nb_components(&rdsu) == 4);

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
	assert(malicious_log.type != RDSU_LOG_YESRANK &&
	       malicious_log.type != RDSU_LOG_NAYRANK);
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
NO_COV void test_rdsu_rewind_clear(void) {
	t_rdsu rdsu;

	rdsu_init(&rdsu, 10);
	assert(rdsu.change_log == NULL);
	assert(rdsu.reserved_logs == NULL);
	rdsu_union(&rdsu, 1, 2);
	rdsu_union(&rdsu, 0, 2); // 0-1-2 3 4
	rdsu_union(&rdsu, 3, 4); // 0-1-2 3-4
	assert(rdsu.change_log != NULL);
	rdsu_rewind(&rdsu);
	assert(rdsu.reserved_logs != NULL);
	rdsu_rewind_clear(&rdsu);
	assert(rdsu.change_log == NULL);
	assert(rdsu.reserved_logs == NULL);
	assert(rdsu_rewind(&rdsu) == false);
	rdsu_rewind_clear(NULL);
	rdsu_destroy(&rdsu);
	return;
}
NO_COV void test_rdsui_log_reuse(void) {
	t_rdsu rdsu;

	rdsu_init(&rdsu, 10);
	assert(rdsu.change_log == NULL);
	assert(rdsu.reserved_logs == NULL);

	alloc_reset_counter();
	assert(rdsu_union(&rdsu, 0, 1) == true);
	assert(nb_alloc == 1); // first log has to be allocated
	assert(rdsu.change_log != NULL);
	assert(rdsu.reserved_logs == NULL);

	// rewinding pops the log to the reserved stack (no alloc/free)
	alloc_reset_counter();
	assert(rdsu_rewind(&rdsu) == true);
	assert(nb_alloc == 0);
	assert(rdsu.change_log == NULL);
	assert(rdsu.reserved_logs != NULL);

	// a new linking re-uses the reserved log, forbidding allocation proves it
	alloc_set_call_limit(0);
	assert(rdsu_union(&rdsu, 0, 1) == true);
	assert(rdsu.change_log != NULL);
	assert(rdsu.reserved_logs == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	// once the reserve is empty an allocation is required again
	alloc_set_call_limit(0);
	assert(rdsu_union(&rdsu, 2, 3) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	rdsu_destroy(&rdsu);
	assert(rdsu.change_log == NULL);
	assert(rdsu.reserved_logs == NULL);
	return;
}
