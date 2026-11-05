#ifndef C_MINI__DSU_CONVERT
#define C_MINI__DSU_CONVERT

#include "dsu/cdsu/compact_disjointed_set_union.h"
#include "dsu/rdsu/rewindable_disjointed_set_union.h"

t_cm_cdsu *cdsu_new_from_rdsu(const t_cm_rdsu *src);
t_cm_cdsu *cdsu_init_from_rdsu(t_cm_cdsu *to, const t_cm_rdsu *src);
t_cm_rdsu *cdsu_to_rdsu(t_cm_cdsu **src);

t_cm_rdsu *rdsu_new_from_cdsu(const t_cm_cdsu *src);
t_cm_rdsu *rdsu_init_from_cdsu(t_cm_rdsu *to, const t_cm_cdsu *src);
t_cm_cdsu *rdsu_to_cdsu(t_cm_rdsu **src);

#endif // !C_MINI__DSU_CONVERT
