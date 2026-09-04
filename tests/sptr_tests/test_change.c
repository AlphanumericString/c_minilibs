#include <assert.h>
#include <sptr/smart_pointer.h>
#include <stdlib.h>
#include <string.h>

#include "sptr_tests.h"

void test_sptr_strengthen(void) {
	void  *no_leaks;
	t_sptr a;
	t_sptr b;

	a = sptr_make_shared((void *)0xDEAD, NULL);
	b = sptr_make_shared(&a, NULL);
	assert(b.smart_strength == SPTR_WEAK && a.box == b.box && a.box != NULL &&
	       a.smart_strength == SPTR_STRONG);
	assert(sptr_strengthen(&b) == true);
	assert(b.smart_strength == SPTR_STRONG);
	assert(sptr_strengthen(&b) == true);
	assert(b.smart_strength == SPTR_STRONG);
	sptr_delete(&a);
	sptr_delete(&b);
	a = sptr_make_shared((void *)0xDEAD, NULL);
	b = sptr_make_shared(&a, NULL);
	assert(b.smart_strength == SPTR_WEAK && a.box == b.box && a.box != NULL &&
	       a.smart_strength == SPTR_STRONG);
	sptr_delete(&a);
	assert(b.box->sc == 0);
	assert(b.box->raw == NULL);
	assert(sptr_strengthen(&b) == false);
	no_leaks = b.box;
	b.box    = NULL;
	assert(sptr_strengthen(&b) == false);
	assert(sptr_strengthen(NULL) == false);
	b.box = no_leaks;
	sptr_delete(&b);
	return;
}
void test_sptr_weaken(void) {
	void  *no_leaks;
	t_sptr a;
	t_sptr b;

	a = sptr_make_shared((void *)0xDEAD, NULL);
	b = sptr_make_shared(&a, NULL);
	assert(b.smart_strength == SPTR_WEAK && a.box == b.box && a.box != NULL &&
	       a.smart_strength == SPTR_STRONG);
	assert(sptr_weaken(&a) == true);
	assert(a.box == b.box && a.box != NULL);
	assert(a.box->raw == NULL);
	assert(a.smart_strength == SPTR_WEAK);
	assert(sptr_weaken(&a) == true);
	assert(sptr_weaken(&a) == true);
	no_leaks = a.box;
	a.box    = NULL;
	assert(sptr_weaken(NULL) == false);
	assert(sptr_weaken(&a) == false);
	a.box = no_leaks;
	sptr_delete(&a);
	sptr_delete(&b);
	a = sptr_make_uniq((void *)0xDEAD, NULL);
	b = sptr_make_uniq(&a, NULL);
	assert(b.smart_strength == SPTR_WEAK && a.box == b.box && a.box != NULL &&
	       a.smart_strength == SPTR_STRONG);
	assert(sptr_weaken(&a) == false);
	sptr_delete(&a);
	sptr_delete(&b);

	return;
}
void test_sptr_steal(void) {
	t_sptr a;
	t_sptr b;
	t_sptr c = (t_sptr){0};

	a = sptr_make_uniq(strdup("aaaa"), free);
	b = sptr_make_uniq(&a, NULL);
	assert(a.smart_strength == SPTR_STRONG && b.smart_strength == SPTR_WEAK &&
	       a.box && a.box->type == SPTR_UNIQ);
	assert(sptr_steal(&c, &a) == false);
	assert(sptr_steal(NULL, &a) == false);
	assert(sptr_steal(&b, &a) == true);
	assert(a.smart_strength == SPTR_WEAK && b.smart_strength == SPTR_STRONG &&
	       a.box && a.box->type == SPTR_UNIQ);
	c.box = b.box;
	b.box = (void *)0xDEADCAFE;
	assert(sptr_steal(&a, &b) == false);
	sptr_delete(&a);
	b.box = c.box;
	sptr_delete(&b);
	return;
}
