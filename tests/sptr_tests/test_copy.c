#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "sptr_tests.h"

void test_sptr_copy(void) {
	t_sptr src[2];
	t_sptr cpy;
	size_t wc, rc;

	src[0] = sptr_make_shared(strdup("hell"), free);
	src[1] = sptr_copy(&src[0]);
	sptr_weaken(&src[1]);
	rc  = src->box->sc;
	wc  = src->box->wc;
	cpy = sptr_copy(&src[0]);

	assert(cpy.box == src[0].box);
	assert(cpy.smart_strength == src[0].smart_strength &&
	       cpy.smart_strength == SPTR_STRONG);
	assert(wc == src->box->wc && src->box->sc == rc + 1);
	sptr_delete(&cpy);
	cpy = sptr_copy(&src[1]);
	assert(cpy.box == src[1].box);
	assert(cpy.smart_strength == src[1].smart_strength &&
	       cpy.smart_strength == SPTR_WEAK);
	assert(src->box->wc == wc + 1 && src->box->sc == rc);
	sptr_delete(&src[0]);
	sptr_delete(&src[1]);
	sptr_delete(&cpy);
	return;
}
void test_sptr_weak_copy(void) {
	t_sptr src[2];
	t_sptr cpy;
	size_t rc, wc;

	src[0] = sptr_make_shared(strdup("hell"), free);
	src[1] = sptr_copy(&src[0]);
	sptr_weaken(&src[1]);
	rc  = src->box->sc;
	wc  = src->box->wc;
	cpy = sptr_weakcopy(&src[0]);

	assert(cpy.box == src[0].box);
	assert(cpy.smart_strength == SPTR_WEAK);
	assert(src->box->wc == wc + 1 && src->box->sc == rc);
	sptr_delete(&cpy);
	cpy = sptr_weakcopy(&src[1]);
	assert(cpy.box == src[1].box);
	assert(cpy.smart_strength == SPTR_WEAK);
	assert(src->box->wc == wc + 1 && src->box->sc == rc);
	sptr_delete(&src[0]);
	sptr_delete(&src[1]);
	sptr_delete(&cpy);
	return;
}
void test_sptr_strong_copy(void) {
	t_sptr src[2];
	t_sptr cpy;
	size_t rc, wc;

	src[0] = sptr_make_shared(strdup("hell"), free);
	src[1] = sptr_copy(&src[0]);
	sptr_weaken(&src[1]);
	rc  = src->box->sc;
	wc  = src->box->wc;
	cpy = sptr_strongcopy(&src[0]);

	assert(cpy.box == src[0].box);
	assert(cpy.smart_strength == SPTR_STRONG);
	assert(src->box->wc == wc && src->box->sc == rc + 1);
	sptr_delete(&cpy);
	cpy = sptr_strongcopy(&src[1]);
	assert(cpy.box == src[1].box);
	assert(cpy.smart_strength == SPTR_STRONG);
	assert(src->box->wc == wc && src->box->sc == rc + 1);
	sptr_delete(&src[0]);
	sptr_delete(&src[1]);
	sptr_delete(&cpy);
	return;
}
