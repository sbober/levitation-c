#include "libowfat/stralloc.h"
#include "levitation.h"

void mw_page_reset(mw_page *page) {
    page->id = 0;
    stralloc_zero(&page->title);
}
