#ifndef C_MINI__LRUTC
#define C_MINI__LRUTC

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "common_base/fptr.h"
#include "common_base/log.h"

#ifdef C_MINI__TESTS

#	define C_MINI__LRUTC_LOG_SUCCESS C_MINI__LOG_SUCCESS

#	define C_MINI__LRUTC_LOG_FAILLURE C_MINI__LOG_FAIL
#	define C_MINI__LRUTC_LOG_EARG     C_MINI__LOG_F_PREFIX "Bad arguments."
#	define C_MINI__LRUTC_LOG_EALLOC   C_MINI__LOG_F_PREFIX "Allocation failure."
#	define C_MINI__LRUTC_LOG_ECLOCK   C_MINI__LOG_F_PREFIX "Clock error."
#	define C_MINI__LRUTC_LOG_EHEAP                                             \
		C_MINI__LOG_F_PREFIX "Inconsistent heap: node is not tracked."
#	define C_MINI__LRUTC_LOG_ENODEU C_MINI__LOG_F_PREFIX "Unknown node"
#	define C_MINI__LRUTC_LOG_EVICTION                                          \
		"Eviction: key=%p delta (%ld) >= ttl (%lu)"
#	define C_MINI__LRUTC_LOG_UPDATE "Update ttl: %zu->%zu"
#	define C_MINI__LRUTC_LOG_HIT    "Hit: hit=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRUTC_LOG_MISS_LRU                                          \
		C_MINI__LOG_F_PREFIX "LRU-Miss: miss=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRUTC_LOG_MISS_TTL                                          \
		C_MINI__LOG_F_PREFIX                                                   \
		"TTL-Miss: miss=%zu, key:%p, key_len:%zu delta(%lu)"
#	define C_MINI__LRUTC_LOG_UPDATEVAL "Update: key:%p, key_len:%zu"
#	define C_MINI__LRUTC_LOG_DELETEVAL "Delete: key:%p, key_len:%zu"

#	define C_MINI__LRUTC_LOG_FAST_FREE "Fast free via list started"
#	define C_MINI__LRUTC_LOG_LFREE     "Cleanup via list started"
#	define C_MINI__LRUTC_LOG_HFREE     "Cleanup via hash started"
#	define C_MINI__LRUTC_LOG_NODE_LRU  "Node replace from lru ordering"
#	define C_MINI__LRUTC_LOG_NODE_MINH "Node replace from MinHeap ordering"

#endif // !C_MINI__TESTS

// ---------------------------------------------------
// NOTE: ALL TIME IS SET/CONTROLLED IN NS (nanoseconds)
// ---------------------------------------------------

/**
 * \defgroup lrutc Least Recently Used Timed Cache
**/

/**
 * \brief stats format for the cache stats
 * \param misses number of misses
 * \param hit number of hits (aka found)
 * \param expired number of expired key/value pairs (aka misses due to time)
 * \ingroup lrutc
**/
typedef struct {
	size_t misses;
	size_t hit;
	size_t expired;
} t_cm_lrutc_stats;

#ifndef CMINI__T_KEYVALUE_DEFINE
#	define CMINI__T_KEYVALUE_DEFINE

/**
 * \brief the key value pair that lives in the cache
 * \param key pointer to key
 * \param k_len length to read from at pointer key
 * \param value value stored and associated with the key
 * \ingroup lrutc
**/
typedef struct {
	const void *key;
	void       *value;
	size_t      k_len;
} t_cm_keyvalue;

#endif // !CMINI__T_KEYVALUE_DEFINE

#ifdef C_MINI__LRUTC_INTERNAL

#	define C_MINI__LRUTC_HASH_SKIP 10
#	define C_MINI__LRUTC_TTL_DFLT  ((size_t)-1)

/**
 * \def C_MINI__LRUTC_NO_HEAP_SLOT
 * \brief heap_index of a node that is not in the min-heap (popped, or parked in
 * the reserved list). heap_index is only meaningful while it points back to the
 * node in the heap (minHeap[heap_index] == node), anything else must be treated
 * as "not tracked by the heap".
 * \ingroup lrutc
**/
#	define C_MINI__LRUTC_NO_HEAP_SLOT ((size_t)-1)

/**
 * \brief [INTERNAL] [MAY CHANGE] the caches internal node for each key
 * \param kv the key-value pair of the node
 * \param time_emitted the time of emission of the node in ns
 * \param ns_gap the ttl grace period of the node in ns, (size_t)-1 means no ttl
 * \param hash_next next value associated with the same hash (collision
 * resolution)
 * \param heap_index index of the node in the TTL ordered minHeap, or
 * C_MINI__LRUTC_NO_HEAP_SLOT when the node is not in the heap (see the macro)
 * \param lru_prev prev value in the recency queue of values (MRU to LRU)
 * \param lru_next next value in the recency queue of values (MRU to LRU)
 * \note in an ifdef statement in order to not pollute LSP but do not depend on
 * this interface. it MAY CHANGE AT ANY MOMENT WITHOUT ANY NOTICE.
 * \ingroup lrutc
**/
typedef struct s_cm_keycache {
	t_cm_keyvalue         kv;
	uint64_t              time_emitted;
	size_t                ns_gap;
	struct s_cm_keycache *hash_next;
	size_t                heap_index;
	struct s_cm_keycache *lru_prev;
	struct s_cm_keycache *lru_next;
} t_cm_lrutc_keynode;

/**
 * \brief [INTERNAL] [MAY CHANGE] internal view of the cache
 * \note invariant, held between any two public calls and after every internal
 * step that is not a partial insert/remove:
 * `node_nb == #nodes in the buckets == #nodes in the recency list == #occupied
 * heap slots`. counter ownership is single-sided: `lrutci_hp_push` owns the +1,
 * the heap removals (`lrutci_hp_pop`, `lrutci_hp_pop_nth`) own the -1, and the
 * list/bucket helpers never touch the counter. reserved nodes are not live
 * entries and never counted by node_nb.
 * \ingroup lrutc
**/
typedef struct {
	size_t buckets_nb;
	size_t node_nb;

	t_cm_lrutc_keynode **buckets;
	t_cm_lrutc_keynode **minHeap;

	t_cm_lrutc_keynode *MRU;
	t_cm_lrutc_keynode *LRU;

	size_t             reserved_nodes_nb;
	t_cm_lrutc_keynode *reserved_nodes;

	size_t ns_gap;

	void            (*dtor)(void *);
	t_cm_lrutc_stats stats;
} t_cm_lrutc;

#else // || C_MINI__LRU_INTERNAL

/**
 * \brief the lru-cache global structure
 * \param buckets_nb number of buckets (also used as the maximum number of
 * entries to limit over-crowding in the cache)
 * \param node_nb number of entries currently present in the cache
 * \param buckets array of nodes indexed via hash value, collision resolved at
 * node level
 * \param minHeap heap ordered nodes using the TTL (faster insert/remove)
 * \param MRU direct pointer to Most Recently Used node
 * \param LRU direct pointer to Least Recently Used node
 * \param reserved_nodes_nb number of pre-allocated nodes currently reserved
 * \param reserved_nodes singly linked list of pre-allocated, reusable nodes
 * \param ns_gap default ttl grace period for elements (default (size_t)-1 aka
 * inf) in ns
 * \param dtor destructor for stored values. Does not get called on keys. Not
 * mandatory to set.
 * \param stats statistics of the lru-cache (see t_cm_lrutc_stats for more
 * precisions)
 * \note `node_nb` is the single source of truth for the number of live entries:
 * it always equals the number of nodes in the hash buckets, in the recency list
 * and the number of occupied min-heap slots.
 * \ingroup lrutc
**/
typedef struct {
	size_t buckets_nb;
	size_t node_nb;

	void *buckets;
	void *minHeap;

	void *MRU;
	void *LRU;

	size_t reserved_nodes_nb;
	void  *reserved_nodes;

	size_t ns_gap;

	void            (*dtor)(void *);
	t_cm_lrutc_stats stats;
} t_cm_lrutc;

#endif // !C_MINI__LRU_INTERNAL

#ifndef CMINI__T_KVCALLBACK_DEFINE
#	define CMINI__T_KVCALLBACK_DEFINE

/**
 * \brief callback function pointer, true when the operation is successful, elem
 * is always the node, arg is passed as is from up function
 * \param elem pointer to the key node element
 * \param arg argument passed "as is" from user, given from user
 * \return expected to return true on success, false otherwise
 * \ingroup lrutc
**/
typedef bool (*t_cm_kvcallback)(t_cm_keyvalue *elem, void *arg);

#endif // CMINI__T_CALLBAKC_DEFINE

t_cm_lrutc *lrutc_create(size_t capacity, void (*free_er)(void *));
t_cm_lrutc *lrutc_init(t_cm_lrutc *cache, size_t capacity,
                     void (*free_er)(void *));
void       lrutc_destroy(t_cm_lrutc *cache);
void       lrutc_free(t_cm_lrutc **cache);

bool lrutc_put_kv(t_cm_lrutc *c, t_cm_keyvalue kv);
bool lrutc_put(t_cm_lrutc *c, const void *key, size_t k_len, const void *value);
bool lrutc_put_ttl(t_cm_lrutc *c, const void *key, size_t k_len, const void *val,
                  size_t ttl_ns);

void *lrutc_try_get(t_cm_lrutc *cache, const void *key, size_t key_len);
void *lrutc_get(t_cm_lrutc *cache, const void *key, size_t key_len, t_cm_work fun,
               void *arg);
void *lrutc_getttl(t_cm_lrutc *cache, const void *key, size_t key_len,
                  t_cm_work fun, void *arg, size_t ttl_ns);
void *lrutc_get_lru(t_cm_lrutc *cache, const void *key, size_t key_len);

void lrutc_clear(t_cm_lrutc *cache);
int  lrutc_remove(t_cm_lrutc *cache, const void *key, size_t key_len);
bool lrutc_clear_reserved(t_cm_lrutc *cache);
void lrutc_clear_to_reserved(t_cm_lrutc *c);

size_t lrutc_reserve(t_cm_lrutc *cache, size_t nb_nodes);

bool lrutc_has(const t_cm_lrutc *cache, const void *key, size_t key_len);
bool lrutc_has_ttl(const t_cm_lrutc *cache, const void *key, size_t key_len);

bool lrutc_isexpired(const t_cm_lrutc *cache, const void *key, size_t key_len);
void lrutc_cleanup(t_cm_lrutc *cache);
void lrutc_setttl(t_cm_lrutc *cache, size_t ttl_ns);

bool lrutc_is_valid(const t_cm_lrutc *cache);

size_t          lrutc_size(const t_cm_lrutc *cache);
uint64_t        lrutc_hits(const t_cm_lrutc *cache);
uint64_t        lrutc_misses(const t_cm_lrutc *cache);
uint64_t        lrutc_expired(const t_cm_lrutc *cache);
t_cm_lrutc_stats lrutc_stats(const t_cm_lrutc *cache);

size_t lrutc_reserved_nb(const t_cm_lrutc *cache);
bool   lrutc_has_reserved(const t_cm_lrutc *cache);
size_t lrutc_max_reserve(const t_cm_lrutc *cache);

void lrutc_foreach(const t_cm_lrutc *cache, t_cm_kvcallback fn, void *u_data);
void lrutc_foreach_ru_ordered(const t_cm_lrutc *c, t_cm_kvcallback fn,
                             void *u_data);
void lrutc_foreach_ttl_ordered(const t_cm_lrutc *c, t_cm_kvcallback fn,
                              void *u_data);

#ifdef C_MINI__LRUTC_INTERNAL
uint64_t lrutci_timeto64(struct timespec t);
uint64_t lrutci_timenow(void);
uint64_t lrutci_expiry(const t_cm_lrutc_keynode *node);
bool     lrutci_expired(const t_cm_lrutc_keynode *node, uint64_t now);
bool lrutci_key_eq(const void *k1, size_t k1_len, const void *k2, size_t k2_len);
bool lrutci_node_eq(const t_cm_lrutc_keynode *node_1,
                   const t_cm_lrutc_keynode *node_2);
void lrutci_unlink_node(t_cm_lrutc *c, t_cm_lrutc_keynode *node);

/**
 * \brief [INTERNAL] [MAY CHANGE] walk every internal structure and check the
 * cache invariants
 * \param c cache pointer
 * \return true when node_nb matches the bucket chains and the recency list,
 * every live node is tracked by the heap at its advertised index, and every
 * reserved node is marked out of the heap
 * \note O(n) in the number of live entries, guards each bounded walk against
 * cycles
 * \ingroup lrutc
**/
bool lrutci_validate(const t_cm_lrutc *c);

size_t lrutci_hash_djb2(const void *key, size_t size);

t_cm_lrutc_keynode *lrutci_keyinhash(const t_cm_lrutc *c, const void *key,
                                   size_t key_len, size_t *_pos);
t_cm_lrutc_keynode *lrutci_keyinlist(const t_cm_lrutc *c, const void *key,
                                   size_t key_len);

void               lrutci_lru_push(t_cm_lrutc *c, t_cm_lrutc_keynode *node);
t_cm_lrutc_keynode *lrutci_lru_pop(t_cm_lrutc *c, t_cm_lrutc_keynode *node);

t_cm_lrutc_keynode *lrutci_hsh_pop(t_cm_lrutc *c, t_cm_lrutc_keynode *n,
                                 size_t hash);
t_cm_lrutc_keynode *lrutci_rsrvd_pop(t_cm_lrutc *c);
bool               lrutci_rsrvd_push(t_cm_lrutc *c, t_cm_lrutc_keynode *n);

void               lrutci_hp_push(t_cm_lrutc *c, t_cm_lrutc_keynode *value);
t_cm_lrutc_keynode *lrutci_hp_pop(t_cm_lrutc *c);
t_cm_lrutc_keynode *lrutci_hp_pop_nth(t_cm_lrutc *c, size_t n);
void               lrutci_hp_rebuild(t_cm_lrutc *c);

#endif // !C_MINI__LRUTC_INTERNAL

#endif // !C_MINI__LRUTC
