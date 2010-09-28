#include "libowfat/stralloc.h"
#include "levitation.h"

void mw_revision_reset(mw_revision *rev) {
    rev->id = 0;
    rev->user_id = 0;
    rev->minor = 0;
    stralloc_zero(&rev->text);
    stralloc_zero(&rev->comment);
    rev->timestamp.x = 0;
}
