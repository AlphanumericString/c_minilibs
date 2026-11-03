#include <assert.h>
#include <stdlib.h>

#include "sptr_tests.h"

static int  g_fbox_dtor_calls = 0;
static void fbox_dt(void *p) {
	(void)p;
	g_fbox_dtor_calls++;
}

NO_COV void test_fbox_get_count(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_rc0  = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 0, .wc = 0};
	t_fbox b1_rc1  = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 1, .wc = 0};
	t_fbox b2_rc42 = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 42, .wc = 0};

	size_t res[] = {
	    sptr_fb_get_count(&b0_rc0),
	    sptr_fb_get_count(&b1_rc1),
	    sptr_fb_get_count(&b2_rc42),
	    sptr_fb_get_count(NULL),
	};

	assert(res[0] == 0);
	assert(res[1] == 1);
	assert(res[2] == 42);
	assert(res[3] == 0);
}

NO_COV void test_fbox_increment(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_rc0 = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 0, .wc = 0};
	t_fbox b1_rc1 = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 1, .wc = 0};

	bool res[] = {
	    sptr_fb_increment(NULL),
	    sptr_fb_increment(&b0_rc0),
	    sptr_fb_increment(&b1_rc1),
	};

	assert(res[0] == false);
	assert(res[1] == false);
	assert(res[2] == true);
	assert(!sptr_fb_get_count(&b0_rc0));
	assert(sptr_fb_get_count(&b1_rc1) == 2);
}

NO_COV void test_fbox_decrement(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_rc0  = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 0, .wc = 0};
	t_fbox b1_rc2  = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 2, .wc = 0};
	t_fbox b2_rc1  = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 1, .wc = 0};
	t_fbox b3_nodt = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};

	g_fbox_dtor_calls = 0;

	bool res[] = {
	    sptr_fb_decrement(NULL),     sptr_fb_decrement(&b0_rc0),
	    sptr_fb_decrement(&b1_rc2),  sptr_fb_decrement(&b2_rc1),
	    sptr_fb_decrement(&b3_nodt),
	};

	assert(res[0] == false);
	assert(res[1] == false);
	assert(res[2] == true);
	assert(res[3] == true);
	assert(res[4] == true);

	// not decremented when rc was 0
	assert(sptr_fb_get_count(&b0_rc0) == 0);
	// decremented but dtor not called while rc stays above 0
	assert(sptr_fb_get_count(&b1_rc2) == 1);
	// reached 0 -> dtor called and data / dtor nulled
	assert(sptr_fb_get_count(&b2_rc1) == 0);
	assert(!b2_rc1.data && !b2_rc1.dtor);
	assert(g_fbox_dtor_calls == 1);
	// reached 0 with a NULL dtor -> must not crash
	assert(sptr_fb_get_count(&b3_nodt) == 0);
	assert(!b3_nodt.data && !b3_nodt.dtor);
}

NO_COV void test_fbox_get_data(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_ok   = (t_fbox){.data = data, .dtor = fbox_dt, .rc = 1, .wc = 0};
	t_fbox b1_null = (t_fbox){.data = NULL, .dtor = fbox_dt, .rc = 1, .wc = 0};

	assert(sptr_fb_get_data(&b0_ok) == true);
	assert(sptr_fb_get_data(&b1_null) == false);
}

NO_COV void test_fbox_init(void) {
	void *data = (void *)0xDEAD;

	t_fbox  box = (t_fbox){.data = NULL, .dtor = NULL, .rc = 42, .wc = 42};
	t_fbox *res = sptr_fb_init(&box, data, fbox_dt);

	assert(res == &box);
	assert(box.data == data);
	assert(box.dtor == fbox_dt);
	assert(sptr_fb_get_count(&box) == 0);
	assert(sptri_wc_get(&box) == 0);

	res = sptr_fb_init(NULL, data, fbox_dt);
	assert(res == NULL);
}

NO_COV void test_fbox_new(void) {
	void *data = (void *)0xDEAD;

	t_fbox *res = sptr_fb_new(data, fbox_dt);

	assert(res != NULL);
	assert(res->data == data);
	assert(res->dtor == fbox_dt);
	assert(sptr_fb_get_count(res) == 0);
	assert(sptri_wc_get(res) == 0);

	free(res);
}
