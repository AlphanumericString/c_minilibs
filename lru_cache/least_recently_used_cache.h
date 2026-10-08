#ifndef C_MINI__LRUC
#define C_MINI__LRUC

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifdef C_MINI__TESTS
#	include <stdio.h>

#	define DBG(fmt, ...)                                                      \
		do {                                                                   \
			printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                        \
			       __func__ __VA_OPT__(, ) __VA_ARGS__);                       \
			fflush(stdout);                                                    \
		} while (0)

#	define LOG(fmt, ...)                                                      \
		do {                                                                   \
			printf("\e[32m[LOG]\e[0m: %s: " fmt "\n",                          \
			       __func__ __VA_OPT__(, ) __VA_ARGS__);                       \
			fflush(stdout);                                                    \
		} while (0)

#	define DBG_POS                                                            \
		do {                                                                   \
			DBG(" %s:%d \e[33;4m[PATH]\e[0m", __FILE__, __LINE__);             \
			fflush(stdout);                                                    \
		} while (0)

#	define C_MINI__LRUC_LOG_SUCCESS  "Success"
#	define C_MINI__LRUC_LOG_ENODEU   "Unknown node"
#	define C_MINI__LRUC_LOG_EARG     "Bad arguments"
#	define C_MINI__LRUC_LOG_EALLOC   "Allocation failure"
#	define C_MINI__LRUC_LOG_ECLOCK   "Clock error"
#	define C_MINI__LRUC_LOG_FAILLURE "Failure"
#	define C_MINI__LRUC_LOG_EVICTION                                          \
		"Eviction: key=%p delta (%ld) >= ttl (%ld)"
#	define C_MINI__LRUC_LOG_UPDATE   "Update ttl: %ld->%ld"
#	define C_MINI__LRUC_LOG_HIT      "Hit: hit=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_MISS_LRU "LRU-Miss: miss=%zu, key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_MISS_TTL                                          \
		"TTL-Miss: miss=%zu, key:%p, key_len:%zu delta(%ld)"
#	define C_MINI__LRUC_LOG_UPDATEVAL "Update: key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_CREATEVAL "Create: key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_DELETEVAL "Delete: key:%p, key_len:%zu"
#	define C_MINI__LRUC_LOG_FAST_FREE "Fast free via list started"
#	define C_MINI__LRUC_LOG_LFREE     "Cleanup via list started"
#	define C_MINI__LRUC_LOG_HFREE     "Cleanup via hash started"
#	define C_MINI__LRUC_LOG_NODE_LRU  "Node replace from lru ordering"
#	define C_MINI__LRUC_LOG_NODE_MINH "Node replace from MinHeap ordering"

#else // || C_MINI__TESTS

#	ifndef LOG
#		define LOG(fmt, ...) (void)0;
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...) (void)0;
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS (void)0;
#	endif // !DBG_POS

#endif // !C_MINI__TESTS

// ---------------------------------------------------
// NOTE: ALL TIME IS SET/CONTROLED IN MS (miliseconds)
// ---------------------------------------------------

/**
 * \defgroup lruc Least Recently Used Cache (with ttl support)
**/

/**
 * \brief stats format for the cache
 * \param misses number of misses
 * \param hit number of hit (aka found)
 * \param expired number of expired key/value pairs (aka misses due to time)
 * \ingroup lruc
**/
typedef struct {
	size_t misses;
	size_t hit;
	size_t expired;
} t_lruc_stats;

/**
 * \brief the key value pair that enters in the cache
 * \param key pointer to key
 * \param k_len length to read from at pointer key
 * \param value stored value in associated to key
 * \ingroup lruc
**/
typedef struct {
	const void *key;
	void       *value;
	size_t      k_len;
} t_keyvalue;

#ifdef C_MINI__INTERNAL

#	define C_MINI__LRUC_HASH_SKIP 10
#	define C_MINI__LRUC_TTL_DFLT  LONG_MAX

/**
 * \brief [INTERNAL] [MAY CHANGE]
 * \param kv the key-value pair of the node
 * \param time the time of emission of the node in ms
 * \param hash_next next value associated with the same hash (collision
 * resolution)
 * \param heap_index index of the node in the TTL ordered minHeap
 * \param lru_prev prev value in the recency queue of values (MRU to LRU)
 * \param lru_next next value in the recency queue of values (MRU to LRU)
 * \note in ifdef statement in order to not polute LSP but do not depend on this
 * interface. it MAY CHANGE AT ANY MOMEMENT WHITHOUT ANY NOTICE.
 * \ingroup lruc
**/
typedef struct s_keycache {
	t_keyvalue         kv;
	uint64_t           time;
	struct s_keycache *hash_next;
	size_t             heap_index;
	struct s_keycache *lru_prev;
	struct s_keycache *lru_next;
} t_lruc_keynode;

typedef struct {
	size_t           buckets_nb;
	size_t           node_nb;
	t_lruc_keynode **buckets;
	t_lruc_keynode **minHeap;

	t_lruc_keynode *MRU;
	t_lruc_keynode *LRU;

	uint64_t ttl_glob;

	void         (*dtor)(void *);
	t_lruc_stats stats;
} t_lruc;

#else

/**
 * \brief the lru-cache global structure
 * \param buckets_nb number of bucket (also used as max numbr of entries to
 * limit over-crowding in cache)
 * \param node_in_use number of entries currently present in the cache
 * \param buckets array of nodes indexed via hash value, collision resolved at
 * node level
 * \param minHeap heap ordered nodes using the TTL (faster insert/remove)
 * \param MRU direct pointer to Most Recently Used node
 * \param LRU direct pointer to Least Recently Used node
 * \param ttl_glob default ttl grace period for elements (default LONG_MAX aka
 * inf) in ms
 * \param dtor destructor for stored values. Does not get called on keys. Not
 * mandatory to set.
 * \param stats statistics of the lru-cache (see t_lruc_stats for more
 * precisions)
 * \ingroup lruc
**/
typedef struct {
	size_t buckets_nb;
	size_t node_nb;
	void **buckets;
	void **minHeap;

	void *MRU;
	void *LRU;

	long ttl_glob;

	void         (*dtor)(void *);
	t_lruc_stats stats;
} t_lruc;

#endif // C_MINI__LRU_INTERNAL

#ifndef CMINI__T_KVCALLBACK_DEFINE
#	define CMINI__T_KVCALLBACK_DEFINE

/**
 * \brief  callback function pointer, true operation successfull, elem is always
 * the node, arg is passed as is from up function
 * \param  elem pointer to the element
 * \param  arg  argument passed as is from up function
 * \return expected to return true on success false otherwise
**/
typedef bool (*t_kvcallback)(t_keyvalue *elem, void *arg);

#endif // CMINI__T_CALLBAKC_DEFINE

t_lruc *lruc_create(size_t capacity, void (*free_er)(void *));
t_lruc *lruc_init(t_lruc *cache, size_t capacity, void (*free_er)(void *));
void    lruc_destroy(t_lruc *cache);
void    lruc_free(t_lruc **cache);

// TODO: switch return of all put to bool (true on success)
int lruc_put_kv(t_lruc *cache, t_keyvalue kv);
int lruc_put(t_lruc *cache, const void *key, size_t key_len, const void *value);
int lruc_put_ttl(t_lruc *cache, const void *key, size_t key_len,
                 const void *value, uint64_t ttl_ms);

void *lruc_get(t_lruc *cache, const void *key, size_t key_len);
void *lruc_get_lru(t_lruc *cache, const void *key, size_t key_len);

void lruc_clear(t_lruc *cache);
int  lruc_remove(t_lruc *cache, const void *key, size_t key_len);

bool lruc_has(const t_lruc *cache, const void *key, size_t key_len);

bool lruc_isexpired(t_lruc *cache, const void *key, size_t key_len);
void lruc_cleanup(t_lruc *cache);
void lruc_setttl(t_lruc *cache, long ttl_ms);

size_t       lruc_size(const t_lruc *cache);
uint64_t     lruc_hits(const t_lruc *cache);
uint64_t     lruc_misses(const t_lruc *cache);
uint64_t     lruc_expired(const t_lruc *cache);
t_lruc_stats lruc_stats(const t_lruc *cache);

void lruc_foreach(const t_lruc *cache, t_kvcallback fn, void *u_data);
void lruc_foreach_ru_ordered(const t_lruc *c, t_kvcallback fn, void *u_data);
void lruc_foreach_ttl_ordered(const t_lruc *c, t_kvcallback fn, void *u_data);

#ifdef C_MINI__INTERNAL
uint64_t lruci_timeto64(struct timespec t);
uint64_t lruci_timenow(void);
bool lruci_key_eq(const void *k1, size_t k1_len, const void *k2, size_t k2_len);
bool lruci_node_eq(t_lruc_keynode *node_1, t_lruc_keynode *node_2);
void lruci_unlink_node(t_lruc *c, t_lruc_keynode *node, size_t hash);

size_t lruci_hash_djb2(const void *key, size_t size);

t_lruc_keynode *lruci_keyinhash(const t_lruc *c, const void *key,
                                size_t key_len, size_t *_pos);
t_lruc_keynode *lruci_keyinlist(const t_lruc *c, const void *key,
                                size_t key_len);

void            lruci_lru_push(t_lruc *c, t_lruc_keynode *node);
t_lruc_keynode *lruci_lru_pop(t_lruc *c, t_lruc_keynode *node);

t_lruc_keynode *lruci_hsh_pop(t_lruc *c, t_lruc_keynode *n, size_t hash);
void            lruci_hsh_push(t_lruc *c, t_lruc_keynode *n, size_t hash);

void            lruci_hp_push(t_lruc *c, t_lruc_keynode *value);
t_lruc_keynode *lruci_hp_pop(t_lruc *c);
t_lruc_keynode *lruci_hp_pop_nth(t_lruc *c, size_t n);

#endif // !C_MINI__INTERNAL

// TODO: add `bool lruci_is_valid(const t_lruc *c);`

#endif // !C_MINI__LRUC
