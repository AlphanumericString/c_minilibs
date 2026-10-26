#ifndef C_MINI__XARRAY
#define C_MINI__XARRAY

/*****************************************************************************/
/* xarray:                                                                   */
/*                                                                           */
/*  <xarray>                                                                 */
/*   |                                                                       */
/*   |-trie_slot0                                                            */
/*   |  |                                                                    */
/*   |  |-trie_slot0-0                                                       */
/*   |  |  |                                                                 */
/*   |  |  |-trie_slot0-0-0                                                  */
/*   |  |  |                                                                 */
/*    [sub trie number is sizeof(index) / sqrt_2(trie_lengths)]              */
/*   |  |  |                                                                 */
/*   |  |  ...                                                               */
/*   |  |  |                                                                 */
/*   |  |  \-trie_slot0-0-n                                                  */
/*   |  |                                                                    */
/*   |  ...                                                                  */
/*   |  |                                                                    */
/*   |  \-trie_slot0-n                                                       */
/*   ...                                                                     */
/*   |                                                                       */
/*   \-trie_slotn                                                            */
/*                                                                           */
/* trie using bits of an integer in chunks and sub-trie s, allocate lazy aka */
/* only when needed.*/
/* A xarray a an array like structure allowing an enormous ammount of slots  */
/*  for datas while lazily allocating space for said data.                   */
/*  It works by using a tree like structure with successively embeded tries  */
/*  and using a bit field to only read the path it needs to walk to for      */
/*  accessing a certain slot. eg: xarray of kernel, uses a uint64_t takes    */
/*  chunk of values 6 by 6 allowing for 64 slots per "layer" of trie.        */
/*  Uppon lookup of empty trie allong the path the xarray can either directly*/
/*  return null (pure lookup) or allocate needed tries allong the way (lazy  */
/*  allocation). Uppon deletion of every values in a trie, it is free'd.     */
/* This memory management system allows the trie to remain extremely light   */
/*  while allowing enormous slot range for users.                            */
/* The mark system works a bit differently, it uses a dedicated double array */
/*  of [nb_marks][number of slots per trie as long] on each trie. eg for a   */
/*  trie of 64 it means that you would have 3*1 (8 octets in long and 8      */
/*  octets long, 8*8=64) or only 192 bits long, 24 octets long               */
/*                                                                           */
/*****************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

//
// TODO: read xarray linux doc pickout usefull funcs

#ifndef C_MINI___XA_NB_NODES
#	define C_MINI___XA_NB_NODES 64
#endif
#ifndef C_MINI___XA_NB_MARKS
#	define C_MINI___XA_NB_MARKS 3
#endif

#if !C_MINI___XA_NB_NODES ||                                                   \
    ((C_MINI___XA_NB_NODES & (C_MINI___XA_NB_NODES - 1)) != 0)
#	error "C_MINI___XA_NB_NODES must be a power of 2"
#endif

typedef struct s_xarray {
	struct s_xarray_node *nodes[C_MINI___XA_NB_NODES];
	size_t                max;
	uint8_t marks_metadatas[C_MINI___XA_NB_MARKS][C_MINI___XA_NB_NODES / 8];
} t_xarray;

t_xarray *xa_new(void);

t_xarray *xa_init(t_xarray *to_init);
bool      xai_delete_node(struct s_xarray_node *node);
bool      xa_delete(t_xarray *to_del);
bool      xa_free(t_xarray *to_free);

#endif // !C_MINI__XARRAY
