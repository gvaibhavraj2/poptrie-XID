/*_
 * Copyright (c) 2014-2016 Hirochika Asai <asai@jar.jp>
 * All rights reserved.
 */

#ifndef _POPTRIE_H
#define _POPTRIE_H

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


//#define uint160 uint8_t [20]
#define POPTRIE_S               18
#define POPTRIE_INIT_FIB_SIZE   4096

#define popcnt(v)               __builtin_popcountll(v)


typedef struct uint160_t{
    uint32_t prefix1;
    __uint128_t prefix2;
}XID;

/* Internal node */
typedef struct poptrie_node {
    u64 leafvec;
    u64 vector;
    u32 base0;
    u32 base1;
} poptrie_node_t;

/* Leaf node */
typedef u16 poptrie_leaf_t;

/*
 * Radix tree node for XID's
 */
struct radix_node160 {
    int valid;
    struct radix_node160 *left;
    struct radix_node160 *right;

    /* Next hop */
    XID prefix;
    int len;
    poptrie_leaf_t nexthop;

    /* Propagated route */
    struct radix_node160 *ext;

    /* Mark for update */
    int mark;
};
/*
 * FIB mapping table
 */
struct poptrie_fib {
    void **entries;
    int n;
    int sz;
};


struct poptrie160 {
    /* Root */
    u32 root;

    /* FIB */
    struct poptrie_fib fib;

    /* Memory management */
    poptrie_node_t *nodes;
    poptrie_leaf_t *leaves;
    void *cnodes;
    void *cleaves;

    /* Size */
    int nodesz;
    int leafsz;

    u32 last_base0;
    u32 last_base1;

    /* Direct pointing */
    u32 *dir;
    u32 *altdir;

    /* RIB */
    struct radix_node160 *radix;

    /* Control */
    int _allocated;
};


#ifdef __cplusplus
extern "C" {
#endif

  
    /* in poptrie160.c */
    struct poptrie160 * poptrie160_init(struct poptrie160 *, int, int);
    void poptrie160_release(struct poptrie160 *);
    int poptrie160_route_add(struct poptrie160 *, XID, int, void *);
    int poptrie160_route_change(struct poptrie160 *, XID, int, void *);
    int poptrie160_route_update(struct poptrie160 *, XID, int, void *);
    int poptrie160_route_del(struct poptrie160 *, XID, int);
    void * poptrie160_lookup(struct poptrie160 *, XID);
    void * poptrie160_rib_lookup(struct poptrie160 *, XID);
#ifdef __cplusplus
}
#endif

#endif /* _POPTRIE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
