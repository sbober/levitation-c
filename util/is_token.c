#include "libowfat/byte.h"
#include "libowfat/stralloc.h"
#include "libowfat/str.h"

int is_token(stralloc* sa, const char* sep) {
    int len = str_len(sep);
    return (sa->len > len && byte_equal( sa->s + sa->len - len, len, sep ));
}
