#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache/least_recently_used_cache.h"

static bool print_entry(t_keyvalue *kv, void *arg) {
	(void)arg;
	printf(" %.*s=%s", (int)kv->k_len, (const char *)kv->key,
	       (const char *)kv->value);
	return (true);
}

static void show(t_lruc *c, const char *label) {
	t_lruc_stats s = lruc_stats(c);

	printf("%-22s size=%zu hits=%zu misses=%zu expired=%zu :", label,
	       lruc_size(c), s.hit, s.misses, s.expired);
	lruc_foreach_ru_ordered(c, print_entry, NULL);
	printf("\n");
}

int main(void) {
	t_lruc *cache = lruc_create(3, free);

	lruc_put(cache, "alpha", 5, strdup("A"));
	lruc_put(cache, "beta", 4, strdup("B"));
	lruc_put(cache, "gamma", 5, strdup("C"));
	show(cache, "initial (3 entries)");

	printf("get(alpha) -> %s\n", (char *)lruc_get(cache, "alpha", 5));
	show(cache, "after get(alpha)");

	lruc_put(cache, "delta", 5, strdup("D"));
	show(cache, "after put(delta)");

	printf("has(beta)  = %s\n", lruc_has(cache, "beta", 4) ? "yes" : "no");
	printf("has(delta) = %s\n", lruc_has(cache, "delta", 5) ? "yes" : "no");

	printf("remove(alpha) -> %d\n", lruc_remove(cache, "alpha", 5));
	show(cache, "after remove(alpha)");

	lruc_setttl(cache, 30);
	lruc_put(cache, "short", 5, strdup("S"));
	printf("get(short) before expiry -> %s\n",
	       (char *)lruc_get(cache, "short", 5));
	usleep(60 * 1000);
	printf("get(short) after 60ms    -> %s\n",
	       (char *)lruc_get(cache, "short", 5));
	lruc_cleanup(cache);
	show(cache, "after cleanup");

	lruc_free(&cache);
	return (0);
}
