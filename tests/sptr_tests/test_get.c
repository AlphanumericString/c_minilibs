#include <assert.h>

#include "sptr_tests.h"

void test_sptr_getdata(void) {
	t_smartbox bad = (t_smartbox){.raw  = &test_sptr_getdata,
	                              .dtor = NULL,
	                              .sc   = -1,
	                              .wc   = 13423452,
	                              .type = 33};

	t_smartbox good = (t_smartbox){.raw  = &test_sptr_steal,
	                               .dtor = NULL,
	                               .sc   = 1,
	                               .wc   = 0,
	                               .type = SPTR_SHRD};
	t_sptr     a, *b, c;

	a = (t_sptr){.box = &bad, .smart_strength = -1};
	c = (t_sptr){.box = &good, .smart_strength = -1};
	b = &c;
	assert(sptr_get(&a) == test_sptr_getdata);
	assert(sptr_get(b) == test_sptr_steal);
	assert(sptr_get(&c) == test_sptr_steal);

	return;
}
void test_sptr_get_wc(void) {
	t_smartbox bad = (t_smartbox){.raw  = &test_sptr_getdata,
	                              .dtor = NULL,
	                              .sc   = -1,
	                              .wc   = 13423452,
	                              .type = 33};

	t_smartbox good = (t_smartbox){.raw  = &test_sptr_steal,
	                               .dtor = NULL,
	                               .sc   = 1,
	                               .wc   = 3,
	                               .type = SPTR_SHRD};
	t_sptr     a, *b, c;

	a = (t_sptr){.box = &bad, .smart_strength = -1};
	c = (t_sptr){.box = &good, .smart_strength = -1};
	b = &c;
	assert(sptr_get_wc(&a) == 13423452);
	assert(sptr_get_wc(b) == 3);
	assert(sptr_get_wc(&c) == 3);

	return;
}
void test_sptr_get_sc(void) {
	t_smartbox bad = (t_smartbox){.raw  = &test_sptr_getdata,
	                              .dtor = NULL,
	                              .sc   = (size_t)-1,
	                              .wc   = 13423452,
	                              .type = 33};

	t_smartbox good = (t_smartbox){.raw  = &test_sptr_steal,
	                               .dtor = NULL,
	                               .sc   = 1,
	                               .wc   = 3,
	                               .type = SPTR_SHRD};
	t_sptr     a, *b, c;

	a = (t_sptr){.box = &bad, .smart_strength = -1};
	c = (t_sptr){.box = &good, .smart_strength = -1};
	b = &c;
	assert(sptr_get_sc(&a) == (size_t)-1);
	assert(sptr_get_sc(b) == 1);
	assert(sptr_get_sc(&c) == 1);

	return;
}
void test_sptr_get_free_er(void) {
	t_smartbox bad = (t_smartbox){.raw  = &test_sptr_getdata,
	                              .dtor = (void (*)(void *))0xDEAD,
	                              .sc   = (size_t)-1,
	                              .wc   = 13423452,
	                              .type = 33};

	t_smartbox good = (t_smartbox){.raw  = &test_sptr_steal,
	                               .dtor = (void (*)(void *))0xBEEF,
	                               .sc   = 1,
	                               .wc   = 3,
	                               .type = SPTR_SHRD};
	t_sptr     a, *b, c;

	a = (t_sptr){.box = &bad, .smart_strength = -1};
	c = (t_sptr){.box = &good, .smart_strength = -1};
	b = &c;
	assert(sptr_get_dtor(&a) == (void (*)(void *))0xDEAD);
	assert(sptr_get_dtor(b) == (void (*)(void *))0xBEEF);
	assert(sptr_get_dtor(&c) == (void (*)(void *))0xBEEF);

	return;
}
