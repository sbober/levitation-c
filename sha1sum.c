/* iksemel (XML parser for Jabber)
** Copyright (C) 2000-2003 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "levitation.h"
#include "libowfat/buffer.h"
#include "libowfat/stralloc.h"
#include "libowfat/io.h"
#include "libowfat/textcode.h"

#define MAX 8192

int main () {
    char digest[20];
    char shabuf[42];
    char buf[MAX];
    int len;

    lvtn_SHA_CTX c;
    lvtn_SHA1_Init(&c);

    io_fd(0);
    while ( (len = io_waitread(0, buf, MAX)) > 0 ) {
        lvtn_SHA1_Update(&c, buf, len);
    }
    lvtn_SHA1_Final(digest, &c);
    len = fmt_hexdump(shabuf, digest, 20);
    shabuf[len] = '\0';
    buffer_puts(STDOUT, shabuf);

    buffer_putsflush(STDOUT, "  -\n");

#if 0
	struct lala {
		char *str;
		char *hash;
	} known_hashes[] = {
		{ "abc", "a9993e364706816aba3e25717850c26c9cd0d89d" },
		{ "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		  "84983e441c3bd26ebaae4aa1f95129e5e54670f1" },
		{ NULL, NULL }
	};
	int i = 0;
	char buf[42];

	while (known_hashes[i].str) {
		iks_sha (known_hashes[i].str, buf);
		if (strcmp (buf, known_hashes[i].hash) != 0) {
			printf("SHA1 hash of \"%s\"\n", known_hashes[i].str);
			printf(" Result:   %s\n", buf);
			printf(" Expected: %s\n", known_hashes[i].hash);
			return 1;
		}
		i++;
	}
#endif
    return 0;
}
