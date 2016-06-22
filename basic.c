/*_
 * Copyright (c) 2014-2016 Hirochika Asai <asai@jar.jp>
 * All rights reserved.
 */

#include "poptrie.h"
#include <stdio.h>
#include <stdlib.h>


/* Macro for testing */
#define TEST_FUNC(str, func, ret)                \
    do {                                         \
        printf("%s: ", str);                     \
        if ( 0 == func() ) {                     \
            printf("passed");                    \
        } else {                                 \
            printf("failed");                    \
            ret = -1;                            \
        }                                        \
        printf("\n");                            \
    } while ( 0 )

#define TEST_PROGRESS()                              \
    do {                                             \
        printf(".");                                 \
        fflush(stdout);                              \
    } while ( 0 )


/*
 * Initialization test
 */
static int
test_init(void)
{
    struct poptrie160 *poptrie;

    /* Initialize */
    poptrie = poptrie160_init(NULL, 19, 22);
    if ( NULL == poptrie ) {
        return -1;
    }

    TEST_PROGRESS();

    /* Release */
    poptrie160_release(poptrie);

    return 0;
}

static int
test_lookup(void)
{
    struct poptrie160 *poptrie;
    int ret;
    void *nexthop;
    XID addr;
    addr.prefix1 = (u32)32;
    addr.prefix2 = 0x1c001203;
    /* Initialize */
    poptrie = poptrie160_init(NULL, 19, 22);
    if ( NULL == poptrie ) {
        return -1;
    }

    /* No route must be found */
    if ( NULL != poptrie160_lookup(poptrie,addr) ) {
        return -1;
    }

    XID addr1;
    addr1.prefix1 = (u32)32;
    addr1.prefix2 = 0x1c001200;
    /* Route add */
    nexthop = (void *)1234;
    ret = poptrie160_route_add(poptrie, addr1, 128, nexthop);
    if ( ret < 0 ) {
        /* Failed to add */
        return -1;
    }
    if ( nexthop != poptrie160_lookup(poptrie, addr) ) {
        printf("not correct");
        return -1;
    }

    TEST_PROGRESS();

    /* Route update */
    nexthop = (void *)5678;
    ret = poptrie160_route_update(poptrie, addr1, 128, nexthop);
    if ( ret < 0 ) {
        /* Failed to update */
        return -1;
    }
    if ( nexthop != poptrie160_lookup(poptrie, addr) ) {
        return -1;
    }
    TEST_PROGRESS();

    /* Route delete */
    ret = poptrie160_route_del(poptrie, addr1, 128);
    if ( ret < 0 ) {
        /* Failed to update */
        return -1;
    }
    if ( NULL != poptrie160_lookup(poptrie, addr) ) {
        return -1;
    }
    TEST_PROGRESS();

    /* Release */
    poptrie160_release(poptrie);

    return 0;

}

/*
 * Main routine for the basic test
 */
int
main(int argc, const char *const argv[])
{
    int ret;

    ret = 0;

    /* Run tests */
    TEST_FUNC("init", test_init, ret);
     TEST_FUNC("lookup", test_lookup, ret);
  
    return ret;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
