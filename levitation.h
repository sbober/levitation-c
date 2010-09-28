#ifndef LEVITATION_H
#define LEVITATION_H 1

#include "libowfat/stralloc.h"
#include "libowfat/uint32.h"
#include "libowfat/tai.h"

#include <time.h>

typedef struct {
    stralloc sitename;
    stralloc domain;
    /* this could actually be 2^31 - 1, but no one does that. FLW */
    stralloc *namespaces;
    int nr_ns;
    int max_ns;
} mw_siteinfo;

typedef mw_siteinfo* mw_siteinfo_ptr;

void mw_siteinfo_init(mw_siteinfo *);

typedef struct {
    uint32 id;
    stralloc title;
} mw_page;
typedef mw_page* mw_page_ptr;

void mw_page_init(mw_page *);
void mw_page_reset(mw_page *);

typedef struct {
    uint32 id;
    uint32 user_id;
    int minor;
    stralloc comment;
    stralloc text;
    tai64 timestamp;
} mw_revision;
typedef struct mw_revision* mw_revision_ptr;

void mw_revision_init(mw_revision *);
void mw_revision_reset(mw_revision *);

typedef enum mw_tag_type {
    MW_base = 0,
    MW_case,
    MW_comment,
    MW_contributor,
    MW_generator,
    MW_id,
    MW_ip,
    MW_mediawiki,
    MW_minor,
    MW_namespace,
    MW_namespaces,
    MW_page,
    MW_revision,
    MW_siteinfo,
    MW_sitename,
    MW_text,
    MW_timestamp,
    MW_title,
    MW_username,
    MW_unknown = 63
} mw_tag_t;


#include "libowfat/buffer.h"

#define STDIN buffer_0
#define STDOUT buffer_1
#define STDERR buffer_2

int is_token(stralloc* sa, const char* sep);

int buffer_get_sep_sa( buffer* b, stralloc* sa, const char* sep);

/*
 * SHA1 routine optimized to do word accesses rather than byte accesses,
 * and to avoid unnecessary copies into the context array.
 *
 * This was initially based on the Mozilla SHA1 implementation, although
 * none of the original Mozilla code remains.
 */

typedef struct {
	unsigned long long size;
	unsigned int H[5];
	unsigned int W[16];
} blk_SHA_CTX;

void blk_SHA1_Init(blk_SHA_CTX *ctx);
void blk_SHA1_Update(blk_SHA_CTX *ctx, const void *dataIn, unsigned long len);
void blk_SHA1_Final(unsigned char hashout[20], blk_SHA_CTX *ctx);

#define lvtn_SHA_CTX	blk_SHA_CTX
#define lvtn_SHA1_Init	blk_SHA1_Init
#define lvtn_SHA1_Update	blk_SHA1_Update
#define lvtn_SHA1_Final	blk_SHA1_Final


#endif
