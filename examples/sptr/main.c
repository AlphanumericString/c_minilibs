#include <stdio.h>
#include <stdlib.h>

#include "sptr/smart_pointer.h"

typedef struct {
	int value;
} t_cm_resource;

static int g_alive = 0;

static t_cm_resource *resource_new(int value) {
	t_cm_resource *r = malloc(sizeof(*r));
	if (r) {
		r->value = value;
		g_alive++;
	}
	return (r);
}

static void resource_dtor(void *ptr) {
	g_alive--;
	free(ptr);
}

static void report(const char *label, const t_cm_strong_ptr *s) {
	printf("%-18s value=%d strong=%zu weak=%zu uniq=%s shared=%s\n", label,
	       ((t_cm_resource *)sptr_get(s))->value, sptr_get_rc(s),
	       sptr_get_wc(s), sptr_is_uniq(s) ? "yes" : "no",
	       sptr_is_shared(s) ? "yes" : "no");
}

int main(void) {
	t_cm_strong_ptr s = sptr_make_shared(resource_new(42), resource_dtor);

	sptr_dump(1, &s);
	report("created", &s);

	t_cm_strong_ptr s2 = sptr_make_shared(&s, NULL);
	report("after copy", &s);
	printf("s2 strong count    = %zu\n", sptr_get_rc(&s2));

	t_cm_weak_ptr w = sptr_make_weak(&s);
	printf("weak observes strong=%zu weak=%zu\n", sptr_get_rc(&w),
	       sptr_get_wc(&w));

	t_cm_strong_ptr locked = sptr_make_shared(&w, NULL);
	printf("weak locked to strong (count=%zu, data=%s)\n", sptr_get_rc(&locked),
	       sptr_get(&locked) == sptr_get(&s) ? "same" : "different");

	sptr_delete(&locked);
	sptr_delete(&s2);
	report("unique again", &s);

	t_cm_owned_ptr owner = sptr_make_owned(&s, NULL);
	printf("moved to owned: owner=%s count=%zu weak=%zu (source empty=%s)\n",
	       owner.owner ? "yes" : "no", sptr_get_rc(&owner), sptr_get_wc(&owner),
	       s.data == NULL ? "yes" : "no");

	sptr_delete(&w);
	printf("after weak release: resource alive=%d\n", g_alive);
	sptr_delete(&owner);
	printf("after owner release: resource alive=%d\n", g_alive);

	t_cm_uniq_ptr u  = sptr_make_uniq(resource_new(7), resource_dtor);
	t_cm_uniq_ptr u2 = sptr_copy(&u);
	printf("unique: value=%d source empty=%s\n",
	       ((t_cm_resource *)sptr_get(&u2))->value,
	       u.data == NULL ? "yes" : "no");
	sptr_delete(&u2);
	printf("after unique release: resource alive=%d\n", g_alive);

	return (0);
}
