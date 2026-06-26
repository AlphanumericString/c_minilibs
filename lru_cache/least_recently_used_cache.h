#ifndef C_MINI__CACHE_GENERIC_H
#define C_MINI__CACHE_GENERIC_H

#define C_MINI__TESTS

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>

#define MAX(a, b)         ((a) > (b)) ? (a) : (b)
#define LRU_LOG_EVICTION  "Eviction: key=%p reason:ttl, delta (%ld) >= ttl (%ld)"
#define LRU_LOG_UPDATE    "Update ttl: %ld->%ld)"
#define LRU_LOG_HIT       "Hit: hit=%zu, key:%p, key_len:%zu"
#define LRU_LOG_MISS      "Miss: miss=%zu, key:%p, key_len:%zu"
#define LRU_LOG_UPDATEVAL "Update: key:%p, key_len:%zu"
#define LRU_LOG_CREATEVAL "Create: key:%p, key_len:%zu"
#define LRU_LOG_DELETEVAL "Delete: key:%p, key_len:%zu"
#define LRU_LOG_FAST_FREE "Fast free via list started"
#define LRU_LOG_LFREE	"Cleanup via list started"
#define LRU_LOG_HFREE	"Cleanup via hash started"

#ifdef C_MINI__TESTS
#	include <stdio.h>
#	define DBG(fmt, ...)                                                      \
		printf("\e[33;5m[DBG]\e[0m: %s: " fmt "\n",                            \
		       __func__ __VA_OPT__(, ) __VA_ARGS__)
#	define LOG(fmt, ...)                                                      \
		printf("\e[32m[LOG]\e[0m: %s: " fmt "\n",                              \
		       __func__ __VA_OPT__(, ) __VA_ARGS__)
#	define DBG_POS                                                            \
		do {                                                                   \
			DBG(" %s:%d \e[33;4m[PATH]\e[0m", __FILE__, __LINE__);             \
		} while (0)
#else
#	define DBG(fmt, ...) (void)fmt;
#endif // C_MINI__TESTS

typedef struct {
	size_t misses;
	size_t hit;
	size_t expired;
} cstats_t;

typedef struct s_keycache {
	void  *key;
	void  *value;
	size_t k_len;

	struct timespec    time; // time of emition
	struct s_keycache *prev;
	struct s_keycache *next;
} t_keyCache;

typedef struct {
	size_t       cap;
	size_t       nb;
	t_keyCache **h_arr; // array of allocated nodes ordered via hash for lookup
	t_keyCache  *head;  // MRU
	t_keyCache  *tail;  // LRU

	void (*dtor)(void *);
#ifdef C_MINI__TESTS
	void (*value_printer)(void *value);
#endif // C_MINI__TESTS

	long ttl_glob;

	cstats_t stats;
} t_lruc;

typedef void (*cache_iter_fn)(t_keyCache *kv, void *user_data);

// create
t_lruc *cache_create(size_t capacity, void (*free_er)(void *));
t_lruc *cache_init(t_lruc *cache, size_t cappacity, void (*free_er)(void *));
void    cache_destroy(t_lruc *cache); // destroy sub-ressources not cache itself
size_t  hash_djb2(const void *key, size_t size);

// put
int cache_put_kv(t_lruc *cache, t_keyCache kv);
int cache_put(t_lruc *cache, const void *key, size_t key_len, void *value);
int cache_put_ttl(t_lruc *cache, const void *key, size_t key_len, void *value,
                  uint64_t ttl_ms);

// get
void       *cache_get(t_lruc *cache, const void *key, size_t key_len);
#ifdef C_MINI__LRU_UTILS
t_keyCache *cache_search_keyinhash(const t_lruc *c, const void *key, size_t key_len,
                            size_t *_pos);
t_keyCache *cache_search_keyinlist(const t_lruc *c, const void *key, size_t key_len);
#endif // C_MINI__LRU_UTILS

// rms
void cache_clear(t_lruc *cache); // clear sub ressources
int  cache_remove(t_lruc *cache, const void *key, size_t key_len);

// has
bool cache_has(const t_lruc *cache, const void *key, size_t key_len);

// expired
bool cache_isexpired(t_lruc *cache, const void *key,
                     size_t key_len); //  is key in the cache and expired?
void cache_cleanup(t_lruc *cache);    // call dtor on all expired elements
void cache_setttl(t_lruc *cache, long ttl);

// infos
size_t   cache_size(const t_lruc *cache);
uint64_t cache_hits(const t_lruc *cache);
uint64_t cache_misses(const t_lruc *cache);
uint64_t cache_expired(const t_lruc *cache);
cstats_t cache_stats(const t_lruc *cache);

#ifdef C_MINI__TESTS
void cache_setvalue_dump(t_lruc *c, void (*value_dumper)(void *));
#endif // C_MINI__TESTS

// foreach
void cache_foreach(t_lruc *cache, cache_iter_fn fn, void *user_data);

#ifdef C_MINI__LRU_UTILS

static inline void cache_link_node(t_lruc *c, t_keyCache *node) {
	node->next = c->head;
	if (c->head) c->head->prev = node;
	if (!c->tail) c->tail = node;
	c->head = node;
}
static inline void cache_unlink_node(t_lruc *c, t_keyCache *node) {
	if (node->next) node->next->prev = node->prev;
	if (node->prev) node->prev->next = node->prev;
	if (node == c->tail) c->tail = c->tail->prev;
	if (node == c->head) c->head = c->head->next;
	node->prev = NULL;
	node->next = NULL;
}

static inline long distance_ms(struct timespec a, struct timespec b) {
	time_t sec  = a.tv_sec - b.tv_sec;
	long   nsec = a.tv_nsec - b.tv_nsec;

	if (nsec < 0) {
		sec--;
		nsec += 1000000000L;
	}
	return sec * 1000 + nsec / 1000000;
}

#endif

// TODO: change has-> if >10% filled check via head->tail

#endif // C_MINI__CACHE_GENERIC_H
