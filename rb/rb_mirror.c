#define C_MINI__RB_INTERNAL
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "round_buffer.h"

#ifdef C_MINI__RB_ATOMIC
static atomic_size_t rbi_map_seq = 0;

#	define RBI_MAP_NEXT_SEQ() atomic_fetch_add(&rbi_map_seq, 1)
#else
static size_t rbi_map_seq = 0;

#	define RBI_MAP_NEXT_SEQ() rbi_map_seq++
#endif

/**
 * \brief map nb_bytes (rounded up to a page) twice, back to back, so indices
 * can run past nb_max without wrapping.
 * \param nb_bytes requested byte capacity
 * \param out_array receives the mapping base
 * \param out_size receives the page-rounded byte size of one window
 * \return true on success, false on any failure (nothing is leaked)
 * \note both halves are MAP_SHARED views of the same anonymous shm object,
 * which is what makes the mirror work: a MAP_PRIVATE second view would give
 * two independent copy-on-write windows.
 * \ingroup rb
**/
bool rbi_map_ring(size_t nb_bytes, void **out_array, size_t *out_size) {
	long        ps;
	size_t      page;
	size_t      map_size;
	char        name[128];
	int         fd;
	void       *res;
	const void *lo;
	const void *hi;

	if (!out_array || !out_size || !nb_bytes) return (false);
	ps = sysconf(_SC_PAGESIZE);
	if (ps <= 0) return (false);
	page = (size_t)ps;
	if (nb_bytes > SIZE_MAX - page) return (false);
	map_size = ((nb_bytes + page - 1) / page) * page;
	if (map_size > SIZE_MAX / 2) return (false);
	(void)snprintf(name, sizeof(name), "/cm_mini_rb_%ld_%zu", (long)getpid(),
	               (size_t)RBI_MAP_NEXT_SEQ());
	fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
	if (fd < 0) return (false);
	// the fd keeps the object alive, the name must not outlive it
	shm_unlink(name);
	if (ftruncate(fd, (off_t)map_size) != 0) {
		close(fd);
		return (false);
	}
	// reserve two contiguous windows so the second map fixed at base + size
	res =
	    mmap(NULL, map_size * 2, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (res == MAP_FAILED) {
		close(fd);
		return (false);
	}
	lo = mmap(res, map_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd,
	          0);
	if (lo == MAP_FAILED) {
		munmap(res, map_size * 2);
		close(fd);
		return (false);
	}
	hi = mmap((unsigned char *)res + map_size, map_size, PROT_READ | PROT_WRITE,
	          MAP_SHARED | MAP_FIXED, fd, 0);
	if (hi == MAP_FAILED) {
		munmap(res, map_size * 2);
		close(fd);
		return (false);
	}
	close(fd);
	*out_array = res;
	*out_size  = map_size;
	return (true);
}

/**
 * \brief release a mapping produced by rbi_map_ring
 * \param array mapping base
 * \param map_size one window byte size
 * \ingroup rb
**/
void rbi_unmap_ring(void *array, size_t map_size) {
	if (!array || !map_size) return;
	munmap(array, map_size * 2);
}

/**
 * \brief copy n elements starting at pos out of the buffer into dst
 * \param rb the round buffer
 * \param pos start position, must be < nb_max when rb is not mirrored
 * \param n number of elements
 * \param dst destination buffer, assumed correctly sized
 * \ingroup rb
**/
void rbi_copy_out(const t_cm_rb *rb, size_t pos, size_t n, void *dst) {
	unsigned char *out;
	size_t         first;

	if (!rb || !dst || !n) return;
	out = dst;
	if (rb->mirrored) {
		memcpy(out, rbi_get_pos(rb, pos), n * rb->sz_e);
		return;
	}
	first = rb->nb_max - pos;
	if (n <= first) {
		memcpy(out, rbi_get_pos(rb, pos), n * rb->sz_e);
		return;
	}
	memcpy(out, rbi_get_pos(rb, pos), first * rb->sz_e);
	memcpy(out + first * rb->sz_e, rb->array, (n - first) * rb->sz_e);
}

/**
 * \brief copy n elements from src into the buffer at pos
 * \param rb the round buffer
 * \param pos start position, must be < nb_max when rb is not mirrored
 * \param n number of elements
 * \param src source buffer, assumed to hold n elements
 * \ingroup rb
**/
void rbi_copy_in(t_cm_rb *rb, size_t pos, const void *src, size_t n) {
	const unsigned char *in;
	size_t               first;

	if (!rb || !src) return;
	in = src;
	if (rb->mirrored) {
		memcpy(rbi_get_pos(rb, pos), in, n * rb->sz_e);
		return;
	}
	first = rb->nb_max - pos;
	if (n <= first) {
		memcpy(rbi_get_pos(rb, pos), in, n * rb->sz_e);
		return;
	}
	memcpy(rbi_get_pos(rb, pos), in, first * rb->sz_e);
	memcpy(rb->array, in + first * rb->sz_e, (n - first) * rb->sz_e);
}
