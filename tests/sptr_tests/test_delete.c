#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sptr_tests.h"

void test_sptr_delete_shared(void) {
	t_sptr a    = (t_sptr){0};
	t_sptr b    = (t_sptr){0};
	t_sptr c    = (t_sptr){0};
	void  *data = strdup("data_string");

	//	return (free(data), (void)0);

	a = sptr_make_shared(data, free);
	b = sptr_make_shared(&a, NULL);
	c = sptr_make_shared(&a, NULL);
	sptr_strengthen(&b);
	sptr_weaken(&c);
	// assert state pre-test
	assert(a.smart_strength == SPTR_S && b.smart_strength == SPTR_S &&
	       c.smart_strength == SPTR_W);
	assert(a.box == b.box && a.box == c.box && a.box != NULL);
	assert(a.box->sc == 2);
	assert(a.box->wc == 1);
	assert(a.box->raw == data && a.box->dtor == free);
	assert(a.box->type == SPTR_SHRD);

	assert(sptr_delete(&b) == true);
	assert(b.smart_strength == 0);
	assert(b.box == NULL);
	assert(a.box != NULL && a.box->raw != NULL && a.box == c.box);
	assert(a.box->raw == data && a.box->dtor == free);
	assert(a.box->sc == 1 && a.box->wc == 1);
	assert(a.smart_strength == SPTR_S && c.smart_strength == SPTR_W);

	assert(sptr_delete(&c) == true);
	assert(c.smart_strength == 0 && c.box == NULL);
	assert(a.box != NULL && a.box->raw != NULL);
	assert(a.box->raw == data && a.box->dtor == free);
	assert(a.box->sc == 1 && a.box->wc == 0);
	assert(a.smart_strength == SPTR_S);

	c = sptr_make_shared(&a, NULL);
	sptr_weaken(&c); // weaken to make sure 1sc and 1wc
	assert(a.box != NULL && a.box->raw != NULL && a.box == c.box);
	assert(a.box->raw == data && a.box->dtor == free);
	assert(a.box->sc == 1 && a.box->wc == 1);
	assert(a.smart_strength == SPTR_S && c.smart_strength == SPTR_W);

	assert(sptr_delete(&a) == true);
	assert(a.smart_strength == 0 && a.box == NULL);
	assert(c.box->raw == NULL && c.box->dtor == free);
	assert(c.box->sc == 0 && c.box->wc == 1);
	assert(c.smart_strength == SPTR_W);
	assert(sptr_delete(&c) == true);
	assert(c.smart_strength == 0 && c.box == NULL);

	assert(sptr_delete(NULL) == false);
}

void test_sptr_delete_uniq(void) {
	t_sptr a;
	t_sptr b;
	t_sptr c;
	void  *data = strdup("data_string");

	a = sptr_make_uniq(data, free);
	b = sptr_make_uniq(&a, NULL);
	c = sptr_make_uniq(&a, NULL);
	sptr_weaken(&b);
	sptr_weaken(&c);
	// assert state pre-test
	assert(a.smart_strength == SPTR_S && b.smart_strength == SPTR_W &&
	       c.smart_strength == SPTR_W);
	assert(a.box == b.box && a.box == c.box && a.box != NULL);
	assert(a.box->sc == 1);
	assert(a.box->wc == 2);
	assert(a.box->raw == data && a.box->dtor == free);
	assert(a.box->type == SPTR_UNIQ);

	assert(sptr_delete(&b) == true);
	assert(b.smart_strength == 0);
	assert(b.box == NULL);
	assert(a.box != NULL && a.box->raw != NULL && a.box == c.box);
	assert(a.box->raw == data && a.box->dtor == free);
	assert(a.box->sc == 1 && a.box->wc == 1);
	assert(a.smart_strength == SPTR_S && c.smart_strength == SPTR_W);

	assert(sptr_delete(&c) == true);
	assert(c.smart_strength == 0 && c.box == NULL);
	assert(a.box != NULL && a.box->raw != NULL);
	assert(a.box->raw == data && a.box->dtor == free);
	assert(a.box->sc == 1 && a.box->wc == 0);
	assert(a.smart_strength == SPTR_S);

	c = sptr_make_uniq(&a, NULL);
	sptr_weaken(&c); // weaken to make sure 1sc and 1wc
	assert(a.box != NULL && a.box->raw != NULL && a.box == c.box);
	assert(a.box->raw == data && a.box->dtor == free);
	assert(a.box->sc == 1 && a.box->wc == 1);
	assert(a.smart_strength == SPTR_S && c.smart_strength == SPTR_W);

	assert(sptr_delete(&a) == true);
	assert(a.smart_strength == 0 && a.box == NULL);
	assert(c.box->raw == NULL && c.box->dtor == free);
	assert(c.box->sc == 0 && c.box->wc == 1);
	assert(c.smart_strength == SPTR_W);

	assert(sptr_delete(&c) == true);
	assert(c.smart_strength == 0 && c.box == NULL);

	assert(sptr_delete(NULL) == false);
}
