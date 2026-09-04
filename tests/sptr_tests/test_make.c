#include <assert.h>
#include <sptr/smart_pointer.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sptr_tests.h"

void test_sptr_make_sharedv(void) {
	t_sptr sptr = (t_sptr){0};
	void  *data = (void *)0xDEAD;

	sptr = sptr_make_shared_v(data, NULL);
	assert(sptr.smart_strength != 0 && sptr.box->raw != NULL);
	assert(sptr.box->raw == data);
	assert(sptr.box->sc == 1 && sptr.box->wc == 0);
	assert(sptr.smart_strength == SPTR_STRONG);
	sptr_delete(&sptr);
	assert(sptr.box == NULL);
	assert(sptr.smart_strength == 0);
	sptr = sptr_make_shared_v(NULL, NULL);
	assert(sptr.box == NULL);
	assert(sptr.smart_strength == 0);
	sptr = sptr_make_shared_v(strdup("test"), free);
	assert(strcmp(sptr.box->raw, "test") == 0);
	sptr_delete(&sptr);
}
void test_sptr_make_sharedsptr(void) {
	t_sptr sptr = (t_sptr){0};
	t_sptr src  = sptr_make_shared_v((void *)0xDEAD, NULL);

	sptr = sptr_make_shared_v(NULL, NULL);
	assert(sptr.smart_strength == 0);
	assert(sptr.box == NULL);
	sptr = sptr_make_shared_fromsptr(&src, NULL);
	assert(sptr_get(&sptr) == sptr_get(&src));
	assert(sptr.box->raw == (void *)0xDEAD);
	assert(sptr_get_wc(&sptr) == 1);
	assert(sptr_get_sc(&sptr) == 1);
	assert(sptr.smart_strength == SPTR_WEAK);
	sptr_delete(&sptr);
	assert(sptr.box == NULL);
	assert(sptr.smart_strength == 0);
	sptr = sptr_make_shared_fromsptr(NULL, NULL);
	assert(sptr.box == NULL);
	assert(sptr.smart_strength == 0);
	sptr_delete(&src);
	src  = sptr_make_uniq_v(strdup("test"), free);
	sptr = sptr_make_shared_fromsptr(&src, free);
	assert(strcmp(sptr_get(&sptr), "test") == 0);
	sptr_delete(&src);
	sptr_delete(&sptr);
}

// x------x
// | uniq |
// x------x

void test_sptr_make_uniqv(void) {
	t_sptr uniq_1 = (t_sptr){0};
	t_sptr uniq_2 = (t_sptr){0};

	uniq_1 = sptr_make_uniq(strdup("aaaa"), free);
	assert(uniq_1.box);
	assert(uniq_1.smart_strength == SPTR_STRONG);
	assert(uniq_1.box->type == SPTR_UNIQ);
	assert(uniq_1.box->closure == free);
	assert(uniq_1.box->sc == 1);
	assert(uniq_1.box->wc == 0);
	uniq_2 = sptr_make_uniq(&uniq_1, NULL);
	assert(uniq_2.smart_strength == SPTR_WEAK);
	assert(uniq_2.box);
	assert(uniq_2.box == uniq_1.box);
	assert(uniq_2.box->wc == 1);
	assert(uniq_2.box->sc == 1);
	sptr_delete(&uniq_1);
	assert(uniq_2.box->wc == 1);
	assert(uniq_2.box->sc == 0);
	assert(uniq_2.box->raw == NULL);
	sptr_delete(&uniq_2);
}
void test_sptr_make_uniqsptr(void) {
	t_sptr shrd_src1;
	t_sptr shrd_src2;
	t_sptr sptr;

	shrd_src1 = sptr_make_shared((void *)0xDEAD, NULL);
	shrd_src2 = sptr_make_shared(&shrd_src1, NULL);
	sptr_strengthen(&shrd_src2);

	assert(shrd_src2.box->sc == 2);

	sptr = sptr_make_uniq(&shrd_src1, NULL);
	assert(sptr.smart_strength == 0);
	assert(sptr.box == NULL);
	sptr_delete(&shrd_src2);
	assert(shrd_src1.box->sc == 1);
	sptr = sptr_make_uniq(&shrd_src1, NULL);
	assert(sptr.smart_strength != 0);
	assert(sptr.box != NULL);
	assert(sptr.smart_strength == SPTR_STRONG);
	assert(sptr.box->sc == 1);
	assert(sptr.box->wc == 0);
	assert(shrd_src1.box->type == SPTR_UNIQ);
	sptr_delete(&sptr);
}
