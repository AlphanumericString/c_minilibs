#include <assert.h>

#include "tls_tests.h"

NO_COV void test_vtls_set(void) {
	void       *prev  = NULL;
	void       *adr[] = {(void *)0xDEAD, (void *)0xCAFE, (void *)0xBEEF,
	                     (void *)0xBEAD};
	t_vtls     *new_arr;
	const char *keys[C_MINI__VTLS_START_SIZE + 1] = {"a", "b", "c", "d", "e"};

	assert(vtls_get(keys[0]) == NULL), assert(vtls_get(keys[1]) == NULL);
	// set new elem
	assert(vtls_set(keys[0], adr[0], sizeof(void *)) == true);
	assert(vtls_get(keys[0]) == adr[0]), assert(vtls_get(keys[1]) == NULL);
	// set new elem
	assert(vtls_set(keys[1], adr[0], sizeof(void *)) == true);
	assert(vtls_get(keys[0]) == adr[0]), assert(vtls_get(keys[1]) == adr[0]);
	// update elem (same size)
	assert(vtls_set(keys[1], adr[1], sizeof(void *)) == true);
	assert(vtls_get(keys[0]) == adr[0]), assert(vtls_get(keys[1]) == adr[1]);
	// update elem (diff size)
	assert(vtls_set(keys[1], adr[2], sizeof(char)) == true);
	assert(vtls_get(keys[0]) == adr[0]), assert(vtls_get(keys[1]) == adr[2]);

	assert(vtls_set(keys[2], adr[3], sizeof(void *)) == true);
	assert(vtls_set(keys[3], adr[3], sizeof(void *)) == true);
	assert(vtls_set(keys[4], adr[3], sizeof(void *)) == true);

	assert(vtls_get(keys[0]) == adr[0] && vtls_get(keys[1]) == adr[2] &&
	       vtls_get(keys[2]) == adr[3] && vtls_get(keys[3]) == adr[3] &&
	       vtls_get(keys[3]) == adr[3]);
	for (size_t i = 0; i < C_MINI__VTLS_START_SIZE + 1; i++)
		vtls_rm(keys[i]);

	// err
	assert(vtls_set(NULL, adr[0], sizeof(void *)) == false);

	prev = pthread_getspecific(vtls_key);                      // save prev
	pthread_setspecific(vtls_key, NULL);                       // set to NULL
	assert(vtls_set(keys[0], adr[0], sizeof(void *)) == true); // set val ok
	assert(pthread_getspecific(vtls_key) != NULL); // confirm init ok
	vtls_rm(keys[0]);
	new_arr = pthread_getspecific(vtls_key);
	free(new_arr->data);
	free(new_arr);

	pthread_setspecific(vtls_key, NULL); // set to NULL
	alloc_reset_counter();               // set alloc limits
	alloc_set_call_limit(1);             //
	assert(vtls_set(keys[0], adr[0], sizeof(void *)) == false); // set val ko
	assert(pthread_getspecific(vtls_key) != NULL); // confirm init ok
	// tldr: init new vtls ok, could not grow array for value
	new_arr = pthread_getspecific(vtls_key);
	assert(new_arr->data == NULL);
	if (new_arr->data) free(new_arr->data);
	free(new_arr);

	pthread_setspecific(vtls_key, NULL);
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(vtls_set(keys[0], adr[0], sizeof(void *)) == false); // set val ko
	assert(pthread_getspecific(vtls_key) == NULL); // confirm init ko

	alloc_set_call_limit(ALLOC_DFT_LIMIT); // reset alloc limit
	pthread_setspecific(vtls_key, prev);   // reset vtls

	assert(pthread_getspecific(vtls_key) == prev); // assert reset to prev

	return;
}
