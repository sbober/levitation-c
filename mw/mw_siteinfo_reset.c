#include "libowfat/stralloc.h"
#include "levitation.h"

void mw_siteinfo_reset(mw_siteinfo *si) {
    int i;
    stralloc_zero(&si->sitename);
    stralloc_zero(&si->domain);
    si->nr_ns       = 0;

    for (i = 0; i < si->max_ns; i++) {
        stralloc_zero(&si->namespaces[i]);
    }
}
