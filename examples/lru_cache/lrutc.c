#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache/lrutc/least_recently_used_timed_cache.h"

static bool print_entry(t_cm_keyvalue *kv, void *arg) {
	(void)arg;
	printf(" %.*s=%s", (int)kv->k_len, (const char *)kv->key,
	       (const char *)kv->value);
	return (true);
}

static void show(t_cm_lrutc *c, const char *label) {
	t_cm_lrutc_stats s = lrutc_stats(c);

	printf("%-22s size=%zu hits=%zu misses=%zu expired=%zu :", label,
	       lrutc_size(c), s.hit, s.misses, s.expired);
	lrutc_foreach_ru_ordered(c, print_entry, NULL);
	printf("\n");
}

int main(void) {
	t_cm_lrutc *cache = lrutc_create(3, free);

	lrutc_put(cache, "alpha", 5, strdup("A"));
	lrutc_put(cache, "beta", 4, strdup("B"));
	lrutc_put(cache, "gamma", 5, strdup("C"));
	show(cache, "initial (3 entries)");

	printf("get(alpha) -> %s\n", (char *)lrutc_try_get(cache, "alpha", 5));
	show(cache, "after get(alpha)");

	lrutc_put(cache, "delta", 5, strdup("D"));
	show(cache, "after put(delta)");

	printf("has(beta)  = %s\n", lrutc_has(cache, "beta", 4) ? "yes" : "no");
	printf("has(delta) = %s\n", lrutc_has(cache, "delta", 5) ? "yes" : "no");

	printf("remove(alpha) -> %d\n", lrutc_remove(cache, "alpha", 5));
	show(cache, "after remove(alpha)");

	lrutc_setttl(cache, 30 * 1000000UL);
	lrutc_put(cache, "short", 5, strdup("S"));
	printf("get(short) before expiry -> %s\n",
	       (char *)lrutc_try_get(cache, "short", 5));
	usleep(60 * 1000);
	printf("get(short) after 60ms    -> %s\n",
	       (char *)lrutc_try_get(cache, "short", 5));
	lrutc_cleanup(cache);
	show(cache, "after cleanup");

	lrutc_free(&cache);
	return (0);
}
