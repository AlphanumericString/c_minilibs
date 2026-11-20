#ifndef C_MINI__LRUC
#define C_MINI__LRUC

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common_base/fptr.h"
#include "common_base/log.h"

#ifdef C_MINI__TESTS

#	define C_MINI__LRUC_LOG_SUCCESS  C_MINI__LOG_SUCCESS
#	define C_MINI__LRUC_LOG_FAILLURE C_MINI__LOG_FAIL
#	define C_MINI__LRUC_LOG_EARG     C_MINI__LOG_EARG
#	define C_MINI__LRUC_LOG_EALLOC   C_MINI__LOG_F_PREFIX "Allocation failure."
#	define C_MINI__LRUC_LOG_ENODEU   C_MINI__LOG_F_PREFIX "Unknown node"
#	define C_MINI__LRUC_LOG_EVICTION                                          \
		"Eviction: key=%p, key_len:%zu (cache full)"
#	define C_MINI__LRUC_LOG_UPDATEVAL "Update: key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_DELETEVAL "Delete: key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_HIT       "Hit: hit=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_MISS                                              \
		C_MINI__LOG_F_PREFIX "Miss: miss=%zu, key:%p, key_len:%zu"

#	define C_MINI__LRUC_LOG_LFREE "Cleanup via list started"
#	define C_MINI__LRUC_LOG_HFREE "Cleanup via hash started"

#endif // !C_MINI__TESTS

/**
 * \defgroup lruc Least Recently Used Cache
**/

#ifndef CMINI__T_KEYVALUE_DEFINE
#	define CMINI__T_KEYVALUE_DEFINE

/**
 * \brief the key value pair that lives in the cache
 * \param key pointer to key
 * \param k_len length to read from at pointer key
 * \param value value stored and associated with the key
 * \ingroup lruc
**/
typedef struct {
	const void *key;
	void       *value;
	size_t      k_len;
} t_cm_keyvalue;

#endif // !CMINI__T_KEYVALUE_DEFINE

/**
 * \brief stats format for the cache stats
 * \param misses number of misses (lookups that did not find a key)
 * \param hit number of hits (aka found)
 * \param expired number of key/value pairs dropped to make room for a new one
 * (the cache was full and its least recently used entry was evicted)
 * \ingroup lruc
**/
typedef struct {
	size_t misses;
	size_t hit;
	size_t expired;
} t_cm_lruc_stats;

#ifdef C_MINI__LRUC_INTERNAL

#	define C_MINI__LRUC_HASH_SKIP 10

/**
 * \brief [INTERNAL] [MAY CHANGE] the caches internal node for each key
 * \param kv the key-value pair of the node
 * \param hash_next next value associated with the same hash (collision
 * resolution)
 * \param lru_prev prev value in the recency queue of values (MRU to LRU)
 * \param lru_next next value in the recency queue of values (MRU to LRU)
 * \note in an ifdef statement in order to not pollute LSP but do not depend on
 * this interface. it MAY CHANGE AT ANY MOMENT WITHOUT ANY NOTICE.
 * \ingroup lruc
**/
typedef struct s_cm_lruc_keynode {
	t_cm_keyvalue             kv;
	struct s_cm_lruc_keynode *hash_next;
	struct s_cm_lruc_keynode *lru_prev;
	struct s_cm_lruc_keynode *lru_next;
} t_cm_lruc_keynode;

/**
 * \brief [INTERNAL] [MAY CHANGE] internal view of the cache
 * \note invariant, held between any two public calls:
 * `node_nb == #nodes in the buckets == #nodes in the recency list`. counter
 * ownership is single-sided: the insertion owns +1, `lruci_unlink_node` owns
 * -1, and the list/bucket helpers never touch the counter. reserved nodes are
 * not live entries and never counted by node_nb.
 * \ingroup lruc
**/
typedef struct {
	size_t buckets_nb;
	size_t node_nb;

	t_cm_lruc_keynode **buckets;

	t_cm_lruc_keynode *MRU;
	t_cm_lruc_keynode *LRU;

	size_t             reserved_nodes_nb;
	t_cm_lruc_keynode *reserved_nodes;

	void            (*dtor)(void *);
	t_cm_lruc_stats stats;
} t_cm_lruc;

#else // || C_MINI__LRUC_INTERNAL

/**
 * \brief the lru-cache global structure, the cache is bounded by capacity and
 * evicts its least recently used entry when full
 * \param buckets_nb number of buckets (also used as the maximum number of
 * entries to limit over-crowding in the cache)
 * \param node_nb number of entries currently present in the cache
 * \param buckets array of nodes indexed via hash value, collision resolved at
 * node level
 * \param MRU direct pointer to Most Recently Used node
 * \param LRU direct pointer to Least Recently Used node (the eviction
 * candidate)
 * \param reserved_nodes_nb number of pre-allocated nodes currently reserved
 * \param reserved_nodes singly linked list of pre-allocated, reusable nodes
 * \param dtor destructor for stored values. Called when an entry is evicted,
 * removed, cleared or replaced by an update of its key, never on keys. Not
 * mandatory to set: when NULL the cache just drops the previous value.
 * \param stats statistics of the lru-cache (see t_cm_lruc_stats for more
 * precisions)
 * \note `node_nb` is the single source of truth for the number of live entries:
 * it always equals the number of nodes in the hash buckets and in the recency
 * list.
 * \ingroup lruc
**/
typedef struct {
	size_t buckets_nb;
	size_t node_nb;

	void *buckets;

	void *MRU;
	void *LRU;

	size_t reserved_nodes_nb;
	void  *reserved_nodes;

	void            (*dtor)(void *);
	t_cm_lruc_stats stats;
} t_cm_lruc;

#endif // !C_MINI__LRUC_INTERNAL

#ifndef CMINI__T_KVCALLBACK_DEFINE
#	define CMINI__T_KVCALLBACK_DEFINE

/**
 * \brief callback function pointer, true when the operation is successful, elem
 * is always the node, arg is passed as is from up function
 * \param elem pointer to the key node element
 * \param arg argument passed "as is" from user, given from user
 * \return expected to return true on success, false otherwise
 * \ingroup lruc
**/
typedef bool (*t_cm_kvcallback)(t_cm_keyvalue *elem, void *arg);

#endif // CMINI__T_CALLBAKC_DEFINE

struct s_cm_lruc_keynode;

/**
 * \typedef t_cm_lruc_iter
 * \brief iterator handle over a pure lru cache, walks the recency list
 * \param cache the cache being walked
 * \param cur internal cursor, opaque to the caller
 * \param promote_to_mru when true, every entry yielded by the forward iterator
 * is promoted to most recently used, which effectively reverses the cache
 * order as the walk progresses. the reverse iterator never promotes
 * \param done whether the walk is exhausted, an exhausted handle keeps yielding
 * nothing until lruc_iter_reset
 * \note the handle is a plain value: copy it to fork a walk, reset it to
 * restart one. walk a handle in a single direction, reset it to walk the other
 * way. mutating the cache (put/get/remove/...) during a walk is not supported.
 * \ingroup lruc
**/
typedef struct s_cm_lruc_iter {
	t_cm_lruc                *cache;
	struct s_cm_lruc_keynode *cur;
	bool                      promote_to_mru;
	bool                      done;
} t_cm_lruc_iter;

/**
 * \brief prepare an iterator handle for a cache
 * \param it iterator handle, caller owned
 * \param c cache to walk
 * \param promote_to_mru true to promote every yielded entry (forward only)
 * \ingroup lruc
**/
void lruc_iter_init(t_cm_lruc_iter *it, t_cm_lruc *c, bool promote_to_mru);
/**
 * \brief yield the next entry from most recently used to least recently used
 * \param it iterator handle initialized by lruc_iter_init
 * \param kv set to the yielded key-value pair, NULL once the walk is done
 * \return true while an entry is available, false once the walk is done or on
 * invalid arguments
 * \ingroup lruc
**/
bool lruc_iter(t_cm_lruc_iter *it, t_cm_keyvalue **kv);
/**
 * \brief yield the next entry from least recently used to most recently used
 * \param it iterator handle initialized by lruc_iter_init
 * \param kv set to the yielded key-value pair, NULL once the walk is done
 * \return true while an entry is available, false once the walk is done or on
 * invalid arguments
 * \note never promotes, even when the handle was created with promote_to_mru
 * \ingroup lruc
**/
bool lruc_iter_rev(t_cm_lruc_iter *it, t_cm_keyvalue **kv);
/**
 * \brief rewind an iterator handle, the next call yields the first entry of the
 * walked direction again
 * \param it iterator handle
 * \ingroup lruc
**/
void lruc_iter_reset(t_cm_lruc_iter *it);

t_cm_lruc *lruc_create(size_t capacity, void (*free_er)(void *));
t_cm_lruc *lruc_init(t_cm_lruc *cache, size_t capacity,
                     void (*free_er)(void *));
void       lruc_destroy(t_cm_lruc *cache);
void       lruc_free(t_cm_lruc **cache);

bool lruc_put_kv(t_cm_lruc *c, t_cm_keyvalue kv);
bool lruc_put(t_cm_lruc *c, const void *key, size_t k_len, const void *value);

void *lruc_try_get(t_cm_lruc *cache, const void *key, size_t key_len);
void *lruc_get(t_cm_lruc *cache, const void *key, size_t key_len, t_cm_work fun,
               void *arg);

void lruc_clear(t_cm_lruc *cache);
int  lruc_remove(t_cm_lruc *cache, const void *key, size_t key_len);
bool lruc_clear_reserved(t_cm_lruc *cache);
void lruc_clear_to_reserved(t_cm_lruc *c);

size_t lruc_reserve(t_cm_lruc *cache, size_t nb_nodes);

bool lruc_has(const t_cm_lruc *cache, const void *key, size_t key_len);

bool lruc_is_valid(const t_cm_lruc *cache);

size_t          lruc_size(const t_cm_lruc *cache);
uint64_t        lruc_hits(const t_cm_lruc *cache);
uint64_t        lruc_misses(const t_cm_lruc *cache);
uint64_t        lruc_expired(const t_cm_lruc *cache);
t_cm_lruc_stats lruc_stats(const t_cm_lruc *cache);

size_t lruc_reserved_nb(const t_cm_lruc *cache);
bool   lruc_has_reserved(const t_cm_lruc *cache);
size_t lruc_max_reserve(const t_cm_lruc *cache);

void lruc_foreach(const t_cm_lruc *cache, t_cm_kvcallback fn, void *u_data);
void lruc_foreach_ru_ordered(const t_cm_lruc *c, t_cm_kvcallback fn,
                             void *u_data);

#ifdef C_MINI__LRUC_INTERNAL

void lruci_unlink_node(t_cm_lruc *c, t_cm_lruc_keynode *node);

/**
 * \brief [INTERNAL] [MAY CHANGE] walk every internal structure and check the
 * cache invariants
 * \param c cache pointer
 * \return true when the recency list holds node_nb nodes, every listed node is
 * reachable from its own bucket, neither chain cycles, and reserved nodes carry
 * no live links
 * \note O(n) in the number of live entries, guards each walk against cycles
 * \ingroup lruc
**/
bool lruci_validate(const t_cm_lruc *c);

size_t lruci_hash_djb2(const void *key, size_t size);

t_cm_lruc_keynode *lruci_keyinhash(const t_cm_lruc *c, const void *key,
                                   size_t key_len, size_t *_pos);
t_cm_lruc_keynode *lruci_keyinlist(const t_cm_lruc *c, const void *key,
                                   size_t key_len);

bool lruci_key_eq(const void *k1, size_t k1_len, const void *k2, size_t k2_len);
bool lruci_node_eq(const t_cm_lruc_keynode *node_1,
                   const t_cm_lruc_keynode *node_2);

void               lruci_lru_push(t_cm_lruc *c, t_cm_lruc_keynode *node);
t_cm_lruc_keynode *lruci_lru_pop(t_cm_lruc *c, t_cm_lruc_keynode *node);

t_cm_lruc_keynode *lruci_hsh_pop(t_cm_lruc *c, t_cm_lruc_keynode *n,
                                 size_t hash);
t_cm_lruc_keynode *lruci_rsrvd_pop(t_cm_lruc *c);
bool               lruci_rsrvd_push(t_cm_lruc *c, t_cm_lruc_keynode *n);

#endif // !C_MINI__LRUC_INTERNAL

#endif // !C_MINI__LRUC
