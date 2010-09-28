#include "libowfat/buffer.h"
#include "libowfat/stralloc.h"
#include "levitation.h"

int buffer_get_sep_sa( buffer* b, stralloc* sa, const char* sep ) {
    int i = 0;
    for (;;) {
        char x;
        if ( !stralloc_readyplus(sa, 1 ) ) return -1;
        switch  ( buffer_getc(b, &x) ) {
            case -1: return -1;
            case 0: return 0;
        }
        i++;
        stralloc_append(sa, &x);
        switch ( is_token(sa, sep) ) {
            case -1: return -1;
            case 0: break;
            case 1: return i;
        }
    }
}
