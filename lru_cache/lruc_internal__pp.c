#include <string.h>
#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

/**
 * @brief Push a node to the cache
 * @param c cache pointer
 * @param node node to push
 **/
void lruci_push_lru(t_lruc *c, t_lruc_keynode *node) {
	node->lru_next = c->MRU;
	if (c->MRU) c->MRU->lru_prev = node;
	if (!c->LRU) c->LRU = node;
	c->MRU         = node;
	node->lru_prev = NULL;
}
/**
 * @brief Pop a node from the cache
 * @param c cache pointer
 * @param n node to pop
 **/
t_lruc_keynode *lruci_pop_lru(t_lruc *c, t_lruc_keynode *n) {
	if (n == c->MRU) c->MRU = c->MRU->lru_next;
	if (n == c->LRU) c->LRU = c->LRU->lru_prev;
	if (n->lru_prev) n->lru_prev->lru_next = n->lru_next;
	if (n->lru_next) n->lru_next->lru_prev = n->lru_prev;
	n->lru_prev = NULL;
	n->lru_next = NULL;
	return (n);
}
/**
 * @brief Pop a node from the cache based on TTL
 * @param c cache pointer
 * @param n node to pop
 **/
t_lruc_keynode *lruci_pop_ttl(t_lruc *c, t_lruc_keynode *n) {
	if (n == c->TTL_Min) c->TTL_Min = n->ttl_prev;
	if (n == c->TTL_Max) c->TTL_Max = n->ttl_next;
	if (n->ttl_next) n->ttl_next->ttl_prev = n->ttl_prev;
	if (n->ttl_prev) n->ttl_prev->ttl_next = n->ttl_next;
	n->ttl_prev = NULL;
	n->ttl_next = NULL;
	return (n);
}
/**
 * @brief pushes a node to the correct bucket and resolve colision
 * @param c the cache to push the node into
 * @param n the node to push
 * @param hash the hash index for the bucket to push into
 **/
void lruci_push_hash(t_lruc *c, t_lruc_keynode *n, size_t hash) {
	hash         = hash % c->buckets_nb;
	n->hash_next = NULL;
	if (c->buckets[hash] != n) n->hash_next = c->buckets[hash];
	c->buckets[hash] = n;
}
/**
 * @brief pop a node at index hash % bucket_nb and resolve hash part
 * @param c the cache to pop the node from
 * @param n the node to pop
 * @param hash the index of the bucket to search into
 * @return the node removed from the hash or null if find fail
 **/
t_lruc_keynode *lruci_pop_hash(t_lruc *c, t_lruc_keynode *n, size_t hash) {
	t_lruc_keynode *prev = NULL;
	t_lruc_keynode *it;

	hash = hash % c->buckets_nb;
	for (it = c->buckets[hash]; it; prev = it, it = it->hash_next)
		if (lruci_key_eq(it->kv.key, it->kv.k_len, n->kv.key,
		                     n->kv.k_len) == true)
			break;
	if (!it) return (NULL);
	if (prev) prev->hash_next = it->hash_next;
	if (c->buckets[hash] == n) c->buckets[hash] = n->hash_next;
	n->hash_next = NULL;
	return (it);
}
