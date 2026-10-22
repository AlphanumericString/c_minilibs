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
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#endif // !C_MINI__XARRAY
