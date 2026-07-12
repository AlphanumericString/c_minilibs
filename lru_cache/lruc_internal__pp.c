#define C_MINI__LRU_INTERNAL
#include "least_recently_used_cache.h"

/**
 * @brief Push a node to the cache
 * @param c cache pointer
 * @param node node to push
 **/
void push_lruc(t_lruc *c, t_lruc_keynode *node) {
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
t_lruc_keynode *pop_lruc(t_lruc *c, t_lruc_keynode *n) {
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
t_lruc_keynode *pop_ttl(t_lruc *c, t_lruc_keynode *n) {
	if (n == c->TTL_Min) c->TTL_Min = n->ttl_prev;
	if (n == c->TTL_Max) c->TTL_Max = n->ttl_next;
	if (n->ttl_next) n->ttl_next->ttl_prev = n->ttl_prev;
	if (n->ttl_prev) n->ttl_prev->ttl_next = n->ttl_next;
	n->ttl_prev = NULL;
	n->ttl_next = NULL;
	return (n);
}
