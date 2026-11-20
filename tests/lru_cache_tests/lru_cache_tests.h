#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif

#include <stdint.h>
#include <unistd.h>

#include "common_base/tests.h"

// ttl arguments are expressed in ms for readability, the api expects ns.
//
// LRUC_TIME_SCALE divides every duration the suite uses, ttls and sleeps alike,
// so the margins between a ttl and the sleep meant to outlive it are preserved
// while the suite stays fast (a slow machine or valgrind run sees the same
// ratios). raise it if an expiry assertion ever turns flaky, lower it to go
// faster. use a divisor of 1000000 to keep the division exact.
#define LRUC_TIME_SCALE   20ULL
#define LRUC_MS_TO_NS(ms) ((size_t)(ms) * (1000000ULL / LRUC_TIME_SCALE))

/**
 * \brief sleep for `ms` milliseconds, scaled down by LRUC_TIME_SCALE
 * \param ms wanted duration in ms (before scaling)
**/
static inline void lruc_test_sleep_ms(unsigned ms) {
	usleep((useconds_t)((uint64_t)ms * 1000ULL / LRUC_TIME_SCALE));
}
