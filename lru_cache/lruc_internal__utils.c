#include <string.h>

#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * @brief Search for a key in the cache list
 * @param c cache pointer
 * @param key key to search for
 * @param key_len key length
 * @return pointer to the key node if found, NULL otherwise
 **/
t_lruc_keynode *lruc_i_search_keyinlist(const t_lruc *c, const void *key,
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
t_lruc_keynode *lruc_i_search_keyinhash(const t_lruc *c, const void *key,
                                        size_t key_len, size_t *_out) {
	size_t          hash;
	t_lruc_keynode *node;

	if (!_out) _out = &hash;
	hash = lruc_i_hash_djb2(key, key_len) % c->buckets_nb;
	node = c->buckets[hash];
	for (; node; node = node->hash_next)
		if (lruc_i_is_key_eq(node->kv.key, node->kv.k_len, key, key_len))
			return (*_out = hash, node);
	return (*_out = hash, NULL);
}

/**
 * @brief Unlink a node from the cache
 * @param c cache pointer
 * @param n node to unlink
 * @param h hash of the node
 **/
void lruc_i_unlink_node(t_lruc *c, t_lruc_keynode *n, size_t h) {
	lruc_i_pop_lru(c, n);
	lruc_i_pop_ttl(c, n);
	lruc_i_pop_hash(c, n, h);
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
size_t lruc_i_hash_djb2(const void *key, size_t size) {
	unsigned char *str;
	size_t         hash;

	hash = 5381;
	str  = (unsigned char *)key;
	while (size--)
		hash = (hash << 5) + hash + str[size];
	return (hash);
}
/**
 * @brief convert timespec to uint64_t
 * @param t the time to convert
 * @return the time64_t
 * @note utility function, likely inlined at compiletime
 **/
uint64_t lruc_i_timeto64(struct timespec t) {
	return (t.tv_sec * 1000ULL + t.tv_nsec / 1000000ULL);
}
/**
 * @brief key comparaison for kv nodes.
 * @param k1 the key to check
 * @param k1_len length of the key to check
 * @param k2 the key to compare to
 * @param k2_len lenght of the key to compare to
 * @return true if match false otherwise
 * @note utility function, likely inlined at compiletime, was annoyed to type
 * boiler plate at every checks
 **/
bool lruc_i_is_key_eq(const void *k1, size_t k1_len, const void *k2,
                      size_t k2_len) {
	return (k1_len == k2_len && memcmp(k1, k2, k2_len) == 0);
}
