#include <string.h>
#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

/**
 * @brief Search for a key in the cache list
 * @param c cache pointer
 * @param key key to search for
 * @param key_len key length
 * @return pointer to the key node if found, NULL otherwise
 **/
t_lruc_keynode *cache_search_keyinlist(const t_lruc *c, const void *key,
                                       size_t key_len) {
	t_lruc_keynode *node;

	for (node = c->MRU; node; node = node->lru_next)
		if (node->kv.k_len == key_len &&
		    memcmp(key, node->kv.key, key_len) == 0)
			return (node);
	return (NULL);
}
/**
 * @brief Search for a key in the cache hash table
 * @param c cache pointer
 * @param key key to search for
 * @param key_len key length
 * @param _out pointer to store the index of the key
 * @return pointer to the key node if found, NULL otherwise
 **/
t_lruc_keynode *cache_search_keyinhash(const t_lruc *c, const void *key,
                                       size_t key_len, size_t *_out) {
	size_t i;
	size_t it;

	if (!_out) _out = &it;
	it = hash_djb2(key, key_len) % c->cap;
	for (i = 0; i < c->cap && c->h_arr[it]; it = (it + 1) % c->cap, i++) {
		if (c->h_arr[it]->kv.k_len == key_len &&
		    !memcmp(c->h_arr[it]->kv.key, key, key_len)) {
			return (*_out = it, c->h_arr[it]);
		}
	}
	return (*_out = it, NULL);
}

/**
 * @brief Unlink a node from the cache
 * @param c cache pointer
 * @param n node to unlink
 **/
void cache_unlink_node(t_lruc *c, t_lruc_keynode *n) {
	pop_lruc(c, n);
	pop_ttl(c, n);
}
// Very fast, but not very good
// see http://www.cse.yorku.ca/~oz/hash.html
// See ft_hash_sdbm or ft_hash_fnv1a for less collisions prone hash functions
/**
 * @brief Hash a key using djb2 algorithm
 * @param key key to hash
 * @param size key size
 * @return hash value
 **/
size_t hash_djb2(const void *key, size_t size) {
	unsigned char *str;
	size_t         hash;

	hash = 5381;
	str  = (unsigned char *)key;
	while (size--)
		hash = (hash << 5) + hash + str[size];
	return (hash);
}
uint64_t timeto64(struct timespec t) {
	return (t.tv_sec * 1000ULL + t.tv_nsec / 1000000ULL);
}
