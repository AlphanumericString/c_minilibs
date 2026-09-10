#include <assert.h>
#include <sptr/smart_pointer.h>
#include <stdlib.h>
#include <string.h>

#include "sptr_tests.h"

void test_sptr_make_shared_from_raw(void) {
	void        *value  = (void *)0xDEADBEEF;
	void        *value2 = (void *)0xCAFE;
	t_strong_ptr p1;
	t_strong_ptr p2;
	t_strong_ptr p3;

	p1 = sptr_make_shared(value, NULL);
	assert(p1->data == value);
	assert(p1->dtor == NULL);
	assert(p1->rc == 1);

	p2 = sptr_make_shared(value, value2);
	assert(p2->data == value);
	assert(p2->rc == 1);
	assert(p2->dtor == value2);

	p3 = sptr_make_shared(NULL, value2);
	assert(p3 == NULL);

	free(p2);
	free(p1);
	return;
}
void test_sptr_make_shared_from_weak(void) {
	void   *value  = (void *)0xDEADBEEF;
	void   *value2 = (void *)0xCAFE;
	t_wcbox _src   = (t_wcbox){
        {value, NULL, 1},
        1
    };
	t_weak_ptr src = &_src;
	t_weak_ptr p1;
	t_weak_ptr p2;
	t_weak_ptr p3;

	p1 = sptr_make_shared_from_weak(src, NULL);
	assert(p1->rcb.data == value);
	assert(p1->rcb.dtor == NULL);
	assert(p1->rcb.rc == 1);
	assert(p1->weak_count == 2);

	p2 = sptr_make_shared_from_weak(src, value2);
	assert(p2 != NULL);
	assert(p2->rcb.data == value);
	assert(p2->rcb.dtor == value2);
	assert(p2->rcb.rc == 1);
	assert(p2->weak_count == 3);

	p3 = sptr_make_shared_from_weak(NULL, value2);
	assert(p3 == NULL);
	assert(p2 == p1 && p1 == src);

	return;
}
void test_sptr_make_shared_from_strong(void) {
	void   *value  = (void *)0xDEADBEEF;
	void   *value2 = (void *)0xCAFE;
	t_wcbox _src   = (t_wcbox){
        {value, NULL, 1},
        1
    };
	t_strong_ptr src = (t_strong_ptr)&_src;
	t_weak_ptr   p1;
	t_weak_ptr   p2;
	t_weak_ptr   p3;

	p1 = sptr_make_shared(src, NULL);
	assert(p1->rcb.data == value);
	assert(p1->rcb.dtor == NULL);
	assert(p1->rcb.rc == 1);
	assert(p1->weak_count == 2);

	p2 = sptr_make_shared(src, value2);
	assert(p2->rcb.data == value);
	assert(p2->rcb.dtor == value2);
	assert(p2->rcb.rc == 1);
	assert(p2->weak_count == 3);

	p3 = sptr_make_shared_from_strong(NULL, value2);
	assert(p3 == NULL);
	assert(p2 == p1 && p1 == (t_weak_ptr)src);

	return;
}
void test_sptr_make_shared_from_uniq(void) {
	t_uniq_ptr   u1           = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr   u2           = sptr_make_uniq((void *)0xCAFE, NULL);
	t_uniq_ptr   u2_secondary = sptr_make_uniq(u2, NULL);
	t_strong_ptr s1;
	t_strong_ptr s2;

	s1 = sptr_make_shared_from_uniq(&u1, (void *)0xCAFE);
	assert(s1 && s1->data == (void *)0xDEAD);
	assert(s1->dtor == (void *)0xCAFE);
	assert(s1->rc == 1);
	assert(sptr_is_uniqs(s1) == true);

	s2 = sptr_make_shared_from_uniq(&u2_secondary, NULL); // non-owner
	assert(!s2);

	s1->dtor = NULL; // 0xCAFE not a fptr
	sptr_delete(s1);
	sptr_delete(u2);
	sptr_delete(u2_secondary);
	return;
}

// x------x
// | uniq |
// x------x

void test_sptr_make_uniq_from_raw(void) {
	t_rcbox u1_expected = {(void *)0xDEAD, NULL, 1};
	t_rcbox u2_expected = {(void *)0xCAFE, (void *)0x4012, 1};

	t_uniq_ptr u1 = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2 = sptr_make_uniq((void *)0xCAFE, (void *)0x4012);
	t_uniq_ptr u3 = sptr_make_uniq(NULL, NULL);

	assert(u1.owner == true && u1.rcbox != NULL);
	assert(memcmp(u1.rcbox, &u1_expected, sizeof(u1_expected)) == 0);
	assert(u2.owner == true && u2.rcbox != NULL);
	assert(memcmp(u2.rcbox, &u2_expected, sizeof(u2_expected)) == 0);
	assert(u3.owner == false && u3.rcbox == NULL);
	u2.rcbox->dtor = NULL;
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u3);
	return;
}
void test_sptr_make_uniq_from_strong(void) {
	t_strong_ptr src1   = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr src2_0 = sptr_make_shared((void *)0xCAFE, (void *)0x4012);
	t_strong_ptr src2_1 = sptr_make_shared((void *)0xCAFE, (void *)0x4012);
	t_strong_ptr src2_2 = sptr_make_shared((void *)0xCAFE, (void *)0x4012);

	t_rcbox u1_expected = {(void *)0xDEAD, NULL, 1};
	t_rcbox u2_expected = {(void *)0xCAFE, (void *)0x4012, 1};

	t_uniq_ptr u1 = sptr_make_uniq(src1, NULL);
	t_uniq_ptr u2 = sptr_make_uniq(src2_0, (void *)0x4012);
	t_uniq_ptr u3 = sptr_make_uniq(src2_1, (void *)0x4013);
	t_uniq_ptr u4 = sptr_make_uniq(src2_2, NULL);
	t_uniq_ptr u5 = sptr_make_uniq(NULL, NULL);

	assert(u1.owner == true && u1.rcbox != NULL);
	assert(memcmp(u1.rcbox, &u1_expected, sizeof(u1_expected)) == 0);
	assert(u2.owner == true && u2.rcbox != NULL);
	assert(memcmp(u2.rcbox, &u2_expected, sizeof(u2_expected)) == 0);
	assert(u3.owner == false && u3.rcbox == NULL);
	assert(u4.owner == true && u4.rcbox != NULL);
	assert(memcmp(u4.rcbox, &u2_expected, sizeof(u2_expected)) == 0);
	assert(u5.owner == false && u5.rcbox == NULL);
	src2_1->dtor = NULL;
	sptr_delete(src2_1);

	u2.rcbox->dtor = NULL;
	u4.rcbox->dtor = NULL;
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u4);
	return;
}
void test_sptr_make_uniq_from_uniq(void) {
	t_uniq_ptr u1 = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2 = sptr_make_uniq((void *)0xCAFE, (void *)0x4012);
	t_uniq_ptr u3 = sptr_make_uniq((void *)0xBEEF, (void *)0x4012);

	t_uniq_ptr u1_new = sptr_make_uniq(u1, (void *)0xCAFE);
	t_uniq_ptr u2_new = sptr_make_uniq(u2, (void *)0x4012);
	t_uniq_ptr u3_new = sptr_make_uniq(u3, (void *)0x4013);

	t_rcbox u1_expected = {(void *)0xDEAD, (void *)0xCAFE, 2};
	t_rcbox u2_expected = {(void *)0xCAFE, (void *)0x4012, 2};

	assert(u1_new.owner == false && u1_new.rcbox != NULL);
	assert(memcmp(u1_new.rcbox, &u1_expected, sizeof(u1_expected)) == 0);
	assert(u2_new.owner == false && u2_new.rcbox != NULL);
	assert(memcmp(u2_new.rcbox, &u2_expected, sizeof(u2_expected)) == 0);
	assert(u3_new.owner == false && u3_new.rcbox == NULL);
	u1_new.rcbox->dtor = NULL;
	u2_new.rcbox->dtor = NULL;
	u3.rcbox->dtor     = NULL;
	sptr_delete(u1);
	sptr_delete(u1_new);
	sptr_delete(u2);
	sptr_delete(u2_new);
	sptr_delete(u3);
	return;
}
