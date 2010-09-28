#include <stdlib.h>
#include "libowfat/stralloc.h"
#include "levitation.h"

#define MAX 64
void mw_siteinfo_init(mw_siteinfo *si) {
    int i;

    stralloc_init(&si->sitename);
    stralloc_init(&si->domain);

    si->nr_ns = 0;

    si->namespaces = malloc( sizeof(stralloc) * MAX );
    si->max_ns = MAX;

    for (i = 0; i < MAX; i++) {
        stralloc_init(&si->namespaces[i]);
    }
}

