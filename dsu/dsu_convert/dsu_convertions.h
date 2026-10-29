#ifndef C_MINI__DSU_CONVERT
#define C_MINI__DSU_CONVERT

#include "dsu/cdsu/compact_disjointed_set_union.h"
#include "dsu/rdsu/rewindable_disjointed_set_union.h"

t_cdsu *cdsu_new_from_rdsu(const t_rdsu *src);
t_cdsu *cdsu_init_from_rdsu(t_cdsu *to, const t_rdsu *src);
t_rdsu *cdsu_to_rdsu(t_cdsu **src);

t_rdsu *rdsu_new_from_cdsu(const t_cdsu *src);
t_rdsu *rdsu_init_from_cdsu(t_rdsu *to, const t_cdsu *src);
t_cdsu *rdsu_to_cdsu(t_rdsu **src);

#endif // !C_MINI__DSU_CONVERT
