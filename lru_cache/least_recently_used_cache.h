#ifndef C_MINI__CACHE_GENERIC_H
#define C_MINI__CACHE_GENERIC_H

#define C_MINI__TESTS


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

#define MAX(a, b) ((a) > (b)) ? (a) : (b)

typedef struct {
	size_t misses;
	size_t hit;
	size_t expired;
} cstats_t;

typedef struct s_keycache {
	void  *key;
	void  *value;
	size_t k_len;

	struct timeval             time; // time of emition
	struct s_keycache *prev;
	struct s_keycache *next;
} t_keyCache;

typedef struct {
	size_t       cap;
	size_t       nb;
	t_keyCache **h_arr; // array of allocated nodes ordered via hash for lookup
	t_keyCache  *head;   // MRU
	t_keyCache  *tail;   // LRU

	void   (*dtor)(void *);
	long ttl_glob;

	cstats_t stats;
} t_lruc;

typedef void (*cache_iter_fn)(t_keyCache kv, void *user_data);

// create
t_lruc *cache_create(size_t capacity, void (*free_er)(void *));
t_lruc *cache_init(t_lruc *cache, size_t cappacity, void (*free_er)(void *));
void    cache_destroy(t_lruc *cache);
size_t hash_djb2(const void *key, size_t size) ;

// put
int cache_put_kv(t_lruc *cache, t_keyCache kv);
int cache_put(t_lruc *cache, const void *key, size_t key_len, void *value);
int cache_put_ttl(t_lruc *cache, const void *key, size_t key_len, void *value,
                 uint64_t ttl_ms);

// get
void *cache_get(t_lruc *cache, const void *key, size_t key_len);

// rms
void cache_clear(t_lruc *cache);
int  cache_remove(t_lruc *cache, const void *key, size_t key_len);

// has
bool   cache_has(t_lruc *cache, const void *key, size_t key_len);

// expired
// expired-> is key in the cache and expired?
bool cache_isexpired(t_lruc *cache, const void *key, size_t key_len);
// cleanup -> call dtor on all expired elements
void cache_cleanup(t_lruc *cache);
void cache_setttl(t_lruc *cache, long ttl);

// infos
size_t cache_size(t_lruc *cache);
uint64_t cache_hits(t_lruc *cache);
uint64_t cache_misses(t_lruc *cache);
uint64_t cache_expired(t_lruc *cache);
cstats_t cache_stats(t_lruc *cache);

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

#endif

#endif // C_MINI__CACHE_GENERIC_H
