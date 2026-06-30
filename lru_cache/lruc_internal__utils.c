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
	size_t          hash;
	t_lruc_keynode *node;

	if (!_out) _out = &hash;
	hash = hash_djb2(key, key_len) % c->buckets_nb;
	node = c->buckets[hash];
	for (; node; node = node->hash_next)
		if (is_key_eq(node->kv.key, node->kv.k_len, key, key_len))
			return (*_out = hash, node);
	return (*_out = hash, NULL);
}

/**
 * @brief Unlink a node from the cache
 * @param c cache pointer
 * @param n node to unlink
 * @param h hash of the node
 **/
void cache_unlink_node(t_lruc *c, t_lruc_keynode *n, size_t h) {
	pop_lruc(c, n);
	pop_ttl(c, n);
	pop_hash(c, n, h);
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

bool is_key_eq(const void *k1, size_t k1_len, const void *k2, size_t k2_len) {
	return (k1_len == k2_len && memcmp(k1, k2, k2_len) == 0);
}
