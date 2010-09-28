#include "libowfat/stralloc.h"
#include "levitation.h"

void mw_revision_init(mw_revision *rev) {
    stralloc_init(&rev->comment);
    stralloc_init(&rev->text);
}
