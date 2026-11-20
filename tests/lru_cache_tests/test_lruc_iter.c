#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

/**
 * \brief walk a cache with the iterator and record the keys in their yielded
 * order
 * \param it initialized iterator handle
 * \param rev true to walk from the least recently used entry
 * \param order output, must hold at least 4 entries
 * \return number of yielded entries
**/
static size_t walk(t_cm_lruc_iter *it, bool rev, const char **order) {
	t_cm_keyvalue *kv;
	size_t         n = 0;

	while (rev ? lruc_iter_rev(it, &kv) : lruc_iter(it, &kv))
		order[n++] = kv->key;
	return (n);
}

NO_COV void test_cm_lruc_iter(void) {
	t_cm_lruc      c;
	t_cm_lruc_iter it;
	t_cm_keyvalue *kv;
	const char    *k1 = "a", *k2 = "b", *k3 = "c";
	void          *v3 = "C";
	const char    *order[4];
	void          *buckets;

	lruc_init(&c, 3, NULL);
	assert(lruc_put(&c, k1, 1, "A"));
	assert(lruc_put(&c, k2, 1, "B"));
	assert(lruc_put(&c, k3, 1, v3)); // MRU: c, b, a :LRU

	// forward walk, no promotion: MRU -> LRU, cache untouched
	lruc_iter_init(&it, &c, false);
	assert(walk(&it, false, order) == 3);
	assert(order[0] == k3 && order[1] == k2 && order[2] == k1);
	assert(c.MRU->kv.key == k3 && c.LRU->kv.key == k1);
	// the handle resets once the walk is done
	assert(lruc_iter(&it, &kv) == false && kv == NULL);

	// reset restarts the walk
	lruc_iter_reset(&it);
	assert(lruc_iter(&it, &kv) == true && kv->key == k3);
	assert(kv->value == v3);

	// reverse walk: LRU -> MRU, never promotes
	lruc_iter_init(&it, &c, true);
	assert(walk(&it, true, order) == 3);
	assert(order[0] == k1 && order[1] == k2 && order[2] == k3);
	assert(c.MRU->kv.key == k3 && c.LRU->kv.key == k1);
	// an exhausted reverse handle stays exhausted
	assert(lruc_iter_rev(&it, &kv) == false && kv == NULL);

	// promoting walk: yields each entry once and reverses the cache order
	lruc_iter_init(&it, &c, true);
	assert(walk(&it, false, order) == 3);
	assert(order[0] == k3 && order[1] == k2 && order[2] == k1);
	assert(c.MRU->kv.key == k1 && c.LRU->kv.key == k3);
	assert(lruci_validate(&c) == true);

	// a second promoting walk yields the reversed order again
	lruc_iter_init(&it, &c, true);
	assert(walk(&it, false, order) == 3);
	assert(order[0] == k1 && order[1] == k2 && order[2] == k3);
	assert(c.MRU->kv.key == k3 && c.LRU->kv.key == k1);
	assert(lruci_validate(&c) == true);

	// empty cache
	lruc_clear(&c);
	lruc_iter_init(&it, &c, false);
	assert(lruc_iter(&it, &kv) == false && kv == NULL);
	assert(lruc_iter_rev(&it, &kv) == false && kv == NULL);

	// bad arguments
	lruc_iter_init(&it, &c, false);
	assert(lruc_iter(NULL, &kv) == false);
	assert(lruc_iter(&it, NULL) == false);
	assert(lruc_iter_rev(NULL, &kv) == false);
	assert(lruc_iter_rev(&it, NULL) == false);
	lruc_iter_init(&it, NULL, false);
	assert(lruc_iter(&it, &kv) == false && kv == NULL);
	assert(lruc_iter_rev(&it, &kv) == false && kv == NULL);
	lruc_iter_init(NULL, &c, false);
	lruc_iter_reset(NULL);

	// invalid cache
	buckets   = c.buckets;
	c.buckets = NULL;
	assert(lruc_iter(&it, &kv) == false && kv == NULL);
	assert(lruc_iter_rev(&it, &kv) == false && kv == NULL);
	c.buckets = buckets;

	lruc_destroy(&c);
	return;
}
/**
 * \brief promoting while walking must not skip nor duplicate entries when the
 * cache has entries behind the cursor
**/
NO_COV void test_cm_lruc_iter_promote_long(void) {
	t_cm_lruc      c;
	t_cm_lruc_iter it;
	t_cm_keyvalue *kv;
	const char    *keys[] = {"a", "b", "c", "d", "e"};
	size_t         seen   = 0;

	lruc_init(&c, 5, NULL);
	for (size_t i = 0; i < 5; i++)
		assert(lruc_put(&c, keys[i], 1, (void *)keys[i]));

	lruc_iter_init(&it, &c, true);
	while (lruc_iter(&it, &kv)) {
		assert(seen < 5);
		assert(kv->key == keys[4 - seen]); // starts at MRU
		seen++;
	}
	assert(seen == 5);
	assert(c.MRU->kv.key == keys[0] && c.LRU->kv.key == keys[4]);
	assert(lruci_validate(&c) == true);
	lruc_destroy(&c);
	return;
}
