#define C_MINI__RDSU_INTERNAL
#include "rdsu_tests.h"

void test_rdsu_save_state(void) {
	t_rdsu_log *log;
	t_rdsu      rdsu;

	rdsu_init(&rdsu, 10);
	log = rdsu_save_state(&rdsu);
	assert(log == NULL);
	rdsu_union(&rdsu, 1, 2);
	log = rdsu_save_state(&rdsu);
	assert(log != NULL);
	rdsu_destroy(&rdsu);
	return;
}
void test_rdsu_rewind(void) {
	t_rdsu rdsu;

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
	rdsu_destroy(&rdsu);
	return;
}
void test_rdsu_rewind_to_state(void) {
	t_rdsu_log *log;
	t_rdsu      rdsu;

	rdsu_init(&rdsu, 5);
	rdsu_union(&rdsu, 1, 2); // 0 1-2 3 4
	log = rdsu_save_state(&rdsu);
	assert(log != NULL);
	assert(rdsu_find(&rdsu, 0) == 0);
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 3) == 3);
	assert(rdsu_find(&rdsu, 4) == 4);
	assert(rdsu_nb_components(rdsu) == 4);
	rdsu_union(&rdsu, 0, 2); // 0-1-2 3 4
	rdsu_union(&rdsu, 3, 4); // 0-1-2 3-4
	assert(rdsu_find(&rdsu, 0) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 3));
	assert(rdsu_find(&rdsu, 0) != rdsu_find(&rdsu, 4));
	assert(rdsu_find(&rdsu, 3) == rdsu_find(&rdsu, 4));
	assert(rdsu_nb_components(rdsu) == 2);
	rdsu_rewind_to_state(&rdsu, log);
	assert(log != NULL);
	assert(rdsu_find(&rdsu, 0) == 0);
	assert(rdsu_find(&rdsu, 1) == rdsu_find(&rdsu, 2));
	assert(rdsu_find(&rdsu, 3) == 3);
	assert(rdsu_find(&rdsu, 4) == 4);
	assert(rdsu_nb_components(rdsu) == 4);
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
	rdsu_destroy(&rdsu);
	return;
}
