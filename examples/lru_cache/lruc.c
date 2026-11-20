#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache/lruc/least_recently_used_cache.h"

static bool print_entry(t_cm_keyvalue *kv, void *arg) {
	(void)arg;
	printf(" %.*s=%s", (int)kv->k_len, (const char *)kv->key,
	       (const char *)kv->value);
	return (true);
}

static void show(t_cm_lruc *c, const char *label) {
	t_cm_lruc_stats s = lruc_stats(c);

	printf("%-24s size=%zu hits=%zu misses=%zu evicted=%zu :", label,
	       lruc_size(c), s.hit, s.misses, s.expired);
	lruc_foreach_ru_ordered(c, print_entry, NULL);
	printf("\n");
}

int main(void) {
	t_cm_lruc     *cache = lruc_create(3, free);
	t_cm_lruc_iter it;
	t_cm_keyvalue *kv;
	size_t         n = 0;

	lruc_put(cache, "alpha", 5, strdup("A"));
	lruc_put(cache, "beta", 4, strdup("B"));
	lruc_put(cache, "gamma", 5, strdup("C"));
	show(cache, "initial (3 entries)");

	printf("get(alpha) -> %s\n", (char *)lruc_try_get(cache, "alpha", 5));
	show(cache, "after get(alpha)");

	// the cache is full, storing delta evicts the least recently used key
	lruc_put(cache, "delta", 5, strdup("D"));
	show(cache, "after put(delta)");
	printf("has(beta)  = %s\n", lruc_has(cache, "beta", 4) ? "yes" : "no");
	printf("has(delta) = %s\n", lruc_has(cache, "delta", 5) ? "yes" : "no");

	// pull iterator, most recently used first, promoting as it walks
	lruc_iter_init(&it, cache, true);
	printf("iterating (promoting):");
	while (lruc_iter(&it, &kv)) {
		printf(" %.*s", (int)kv->k_len, (const char *)kv->key);
		n++;
	}
	printf(" (%zu entries, cache order reversed)\n", n);
	show(cache, "after promoting walk");

	lruc_remove(cache, "alpha", 5);
	show(cache, "after remove(alpha)");

	lruc_free(&cache);
	return (0);
}
