#include "libowfat/stralloc.h"
#include "levitation.h"

void mw_page_init(mw_page *page) {
    stralloc_init(&page->title);
}
