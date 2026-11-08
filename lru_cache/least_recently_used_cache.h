#ifndef C_MINI__LRUC
#define C_MINI__LRUC

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "common_base/fptr.h"
#include "common_base/log.h"

#ifdef C_MINI__TESTS

#	define C_MINI__LRUC_LOG_SUCCESS C_MINI__LOG_SUCCESS

#	define C_MINI__LRUC_LOG_FAILLURE C_MINI__LOG_FAIL
#	define C_MINI__LRUC_LOG_EARG     C_MINI__LOG_F_PREFIX "Bad arguments."
#	define C_MINI__LRUC_LOG_EALLOC   C_MINI__LOG_F_PREFIX "Allocation failure."
#	define C_MINI__LRUC_LOG_ECLOCK   C_MINI__LOG_F_PREFIX "Clock error."
#	define C_MINI__LRUC_LOG_ENODEU   C_MINI__LOG_F_PREFIX "Unknown node"
#	define C_MINI__LRUC_LOG_EVICTION                                          \
		"Eviction: key=%p delta (%ld) >= ttl (%lu)"
#	define C_MINI__LRUC_LOG_UPDATE "Update ttl: %zu->%zu"
#	define C_MINI__LRUC_LOG_HIT    "Hit: hit=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_MISS_LRU                                          \
		C_MINI__LOG_F_PREFIX "LRU-Miss: miss=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_MISS_TTL                                          \
		C_MINI__LOG_F_PREFIX                                                   \
		"TTL-Miss: miss=%zu, key:%p, key_len:%zu delta(%lu)"
#	define C_MINI__LRUC_LOG_UPDATEVAL "Update: key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_CREATEVAL "Create: key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_DELETEVAL "Delete: key:%p, key_len:%zu"

#	define C_MINI__LRUC_LOG_FAST_FREE "Fast free via list started"
#	define C_MINI__LRUC_LOG_LFREE     "Cleanup via list started"
#	define C_MINI__LRUC_LOG_HFREE     "Cleanup via hash started"
#	define C_MINI__LRUC_LOG_NODE_LRU  "Node replace from lru ordering"
#	define C_MINI__LRUC_LOG_NODE_MINH "Node replace from MinHeap ordering"

#endif // !C_MINI__TESTS

// ---------------------------------------------------
// NOTE: ALL TIME IS SET/CONTROLLED IN NS (nanoseconds)
// ---------------------------------------------------

/**
 * \defgroup lruc Least Recently Used Cache (with ttl support)
**/

/**
 * \brief stats format for the cache stats
 * \param misses number of misses
 * \param hit number of hits (aka found)
 * \param expired number of expired key/value pairs (aka misses due to time)
 * \ingroup lruc
**/
typedef struct {
	size_t misses;
	size_t hit;
	size_t expired;
} t_cm_lruc_stats;

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

#ifdef C_MINI__LRUC_INTERNAL

#	define C_MINI__LRUC_HASH_SKIP 10
#	define C_MINI__LRUC_TTL_DFLT  ((size_t)-1)

/**
 * \brief [INTERNAL] [MAY CHANGE] the caches internal node for each key
 * \param kv the key-value pair of the node
 * \param time_emitted the time of emission of the node in ns
 * \param ns_gap the ttl grace period of the node in ns, (size_t)-1 means no ttl
 * \param hash_next next value associated with the same hash (collision
 * resolution)
 * \param heap_index index of the node in the TTL ordered minHeap
 * \param lru_prev prev value in the recency queue of values (MRU to LRU)
 * \param lru_next next value in the recency queue of values (MRU to LRU)
 * \note in an ifdef statement in order to not pollute LSP but do not depend on
 * this interface. it MAY CHANGE AT ANY MOMENT WITHOUT ANY NOTICE.
 * \ingroup lruc
**/
typedef struct s_cm_keycache {
	t_cm_keyvalue         kv;
	uint64_t              time_emitted;
	size_t                ns_gap;
	struct s_cm_keycache *hash_next;
	size_t                heap_index;
	struct s_cm_keycache *lru_prev;
	struct s_cm_keycache *lru_next;
} t_cm_lruc_keynode;

typedef struct {
	size_t buckets_nb;
	size_t node_nb;

	t_cm_lruc_keynode **buckets;
	t_cm_lruc_keynode **minHeap;

	t_cm_lruc_keynode *MRU;
	t_cm_lruc_keynode *LRU;

	size_t             reserved_nodes_nb;
	t_cm_lruc_keynode *reserved_nodes;

	size_t ns_gap;

	void            (*dtor)(void *);
	t_cm_lruc_stats stats;
} t_cm_lruc;

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
 * \param stats statistics of the lru-cache (see t_cm_lruc_stats for more
 * precisions)
 * \ingroup lruc
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
	t_cm_lruc_stats stats;
} t_cm_lruc;

#endif // !C_MINI__LRU_INTERNAL

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

t_cm_lruc *lruc_create(size_t capacity, void (*free_er)(void *));
t_cm_lruc *lruc_init(t_cm_lruc *cache, size_t capacity,
                     void (*free_er)(void *));
void       lruc_destroy(t_cm_lruc *cache);
void       lruc_free(t_cm_lruc **cache);

bool lruc_put_kv(t_cm_lruc *c, t_cm_keyvalue kv);
bool lruc_put(t_cm_lruc *c, const void *key, size_t k_len, const void *value);
bool lruc_put_ttl(t_cm_lruc *c, const void *key, size_t k_len, const void *val,
                  size_t ttl_ns);

void *lruc_try_get(t_cm_lruc *cache, const void *key, size_t key_len);
void *lruc_get(t_cm_lruc *cache, const void *key, size_t key_len, t_cm_work fun,
               void *arg);
void *lruc_getttl(t_cm_lruc *cache, const void *key, size_t key_len,
                  t_cm_work fun, void *arg, size_t ttl_ns);
void *lruc_get_lru(t_cm_lruc *cache, const void *key, size_t key_len);

void lruc_clear(t_cm_lruc *cache);
int  lruc_remove(t_cm_lruc *cache, const void *key, size_t key_len);
bool lruc_clear_reserved(t_cm_lruc *cache);
void lruc_clear_to_reserved(t_cm_lruc *c);

size_t lruc_reserve(t_cm_lruc *cache, size_t nb_nodes);

bool lruc_has(const t_cm_lruc *cache, const void *key, size_t key_len);
bool lruc_has_ttl(const t_cm_lruc *cache, const void *key, size_t key_len);

bool lruc_isexpired(const t_cm_lruc *cache, const void *key, size_t key_len);
void lruc_cleanup(t_cm_lruc *cache);
void lruc_setttl(t_cm_lruc *cache, size_t ttl_ns);

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
void lruc_foreach_ttl_ordered(const t_cm_lruc *c, t_cm_kvcallback fn,
                              void *u_data);

#ifdef C_MINI__LRUC_INTERNAL
uint64_t lruci_timeto64(struct timespec t);
uint64_t lruci_timenow(void);
uint64_t lruci_expiry(const t_cm_lruc_keynode *node);
bool     lruci_expired(const t_cm_lruc_keynode *node, uint64_t now);
bool lruci_key_eq(const void *k1, size_t k1_len, const void *k2, size_t k2_len);
bool lruci_node_eq(const t_cm_lruc_keynode *node_1,
                   const t_cm_lruc_keynode *node_2);
void lruci_unlink_node(t_cm_lruc *c, t_cm_lruc_keynode *node, size_t hash);

size_t lruci_hash_djb2(const void *key, size_t size);

t_cm_lruc_keynode *lruci_keyinhash(const t_cm_lruc *c, const void *key,
                                   size_t key_len, size_t *_pos);
t_cm_lruc_keynode *lruci_keyinlist(const t_cm_lruc *c, const void *key,
                                   size_t key_len);

void               lruci_lru_push(t_cm_lruc *c, t_cm_lruc_keynode *node);
t_cm_lruc_keynode *lruci_lru_pop(t_cm_lruc *c, t_cm_lruc_keynode *node);

t_cm_lruc_keynode *lruci_hsh_pop(t_cm_lruc *c, t_cm_lruc_keynode *n,
                                 size_t hash);
t_cm_lruc_keynode *lruci_rsrvd_pop(t_cm_lruc *c);
bool               lruci_rsrvd_push(t_cm_lruc *c, t_cm_lruc_keynode *n);

void               lruci_hp_push(t_cm_lruc *c, t_cm_lruc_keynode *value);
t_cm_lruc_keynode *lruci_hp_pop(t_cm_lruc *c);
t_cm_lruc_keynode *lruci_hp_pop_nth(t_cm_lruc *c, size_t n);
void               lruci_hp_rebuild(t_cm_lruc *c);

#endif // !C_MINI__LRUC_INTERNAL

#endif // !C_MINI__LRUC
