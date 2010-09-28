#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "iksemel/iksemel.h"
#include "libowfat/io.h"
#include "libowfat/byte.h"
#include "libowfat/stralloc.h"
#include "libowfat/fmt.h"
#include "stack.h"
#include "levitation.h"

typedef struct {
    mw_siteinfo     siteinfo;
    mw_page         page;
    mw_revision     rev;
    stack           in;
    stralloc        cdata;
    int             nr_pages;
    int             nr_revs;
} mw_data;

static struct tagmap {
    char                *tag;
    enum mw_tag_type    type;
} tags_mapping[] = {
    "base",         MW_base,
    "comment",      MW_comment,
    "contributor",  MW_contributor,
    "id",           MW_id,
    "ip",           MW_ip,
    "mediawiki",    MW_mediawiki,
    "minor",        MW_minor,
    "namespace",    MW_namespace,
    "namespaces",   MW_namespaces,
    "page",         MW_page,
    "revision",     MW_revision,
    "siteinfo",     MW_siteinfo,
    "sitename",     MW_sitename,
    "text",         MW_text,
    "timestamp",    MW_timestamp,
    "title",        MW_title,
    "username",     MW_username
};
static int tag_nr = 17;

#define calc_state(s1, s2) \
    ((s1) << 7) + (s2)

#define di_state(to, s1, s2) \
    STATE_ ## to = calc_state(MW_ ## s1, MW_ ## s2)

enum valid_di_states {
    di_state(   siteinfo,   mediawiki,      siteinfo    ),
    di_state(   sitename,   siteinfo,       sitename    ),
    di_state(   sitebase,   siteinfo,       base        ),
    di_state(   namespaces, siteinfo,       namespaces  ),
    di_state(   namespace,  namespaces,     namespace   ),
    di_state(   page,       mediawiki,      page        ),
    di_state(   pageid,     page,           id          ),
    di_state(   title,      page,           title       ),
    di_state(   revision,   page,           revision    ),
    di_state(   revid,      revision,       id          ),
    di_state(   revtime,    revision,       timestamp   ),
    di_state(   comment,    revision,       comment     ),
    di_state(   text,       revision,       text        ),
    di_state(   minor,      revision,       minor       ),
    di_state(   user,       revision,       contributor ),
    di_state(   userid,     contributor,    id          ),
    di_state(   username,   contributor,    username    ),
    di_state(   ip,         contributor,    ip          ),
};

void data_init(mw_data* data) {

    mw_siteinfo_init(   &data->siteinfo );
    mw_page_init(       &data->page );
    mw_revision_init(   &data->rev );
    stack_init(         &data->in, 8 );
    stralloc_init(      &data->cdata );
    data->nr_pages = 0;
    data->nr_revs  = 0;
}

int taghook(void *udata, char *name, char **atts, int type) {
    int ret;
    int tagindex;
    int tagtype;
    mw_data *data = (mw_data *)udata;
    tagindex = tagsearch(tags_mapping, name, tag_nr);
    tagtype = tagindex == -1 ? MW_unknown : tags_mapping[tagindex].type;


    switch(type) {
        case IKS_OPEN:
            ret = handle_open_tag(data, tagtype, atts);
            break;
        case IKS_CLOSE:
            ret = handle_close_tag(data, tagtype, atts);
            break;
        case IKS_SINGLE:
            ret = handle_single_tag(data, tagtype, atts);
            break;
    }
    return ret;
}

int cdatahook(void *udata, char *text, size_t len) {
    mw_data *data = (mw_data *)udata;

    stralloc_catb(&data->cdata, text, len);

    return IKS_OK;
}

int handle_open_tag(mw_data *data, int tagtype, char **atts) {
    switch(tagtype) {
        case MW_siteinfo:
            mw_siteinfo_reset(&data->siteinfo);
            break;
        case MW_page:
            mw_page_reset(&data->page);
            break;
        case MW_revision:
            mw_revision_reset(&data->rev);
            break;
    }

    stralloc_zero(&data->cdata);

    stack_push(&data->in, tagtype);

    return IKS_OK;
}

void save_data(mw_data *);
int handle_single_tag(mw_data *data, int tagtype, char **atts) {
    stack_push(&data->in, tagtype);
    save_data(data);

    stack_pop(&data->in);


    return IKS_OK;
}

void print_progress(mw_data *data) {
    buffer_puts(STDOUT, "progress pages: ");
    buffer_putlong(STDOUT, data->nr_pages);
    buffer_puts(STDOUT, " / revisions: ");
    buffer_putlong(STDOUT, data->nr_revs);
    buffer_putnlflush(STDOUT);
}

void output_revision(mw_data *);
int handle_close_tag(mw_data *data, int tagtype, char **atts) {
    int old_type;

    save_data(data);

    old_type = stack_pop(&data->in);

    if (old_type != tagtype)
        return IKS_BADXML;

    if (old_type == MW_revision) {
        data->nr_revs++;
        output_revision(data);
        /* persist_revision(data); */
    }

    if (old_type == MW_page) {
        data->nr_pages++;
        if (data->nr_pages % 10000 == 0) {
            print_progress(data);
        }
    }

    return IKS_OK;
}

void save_data(mw_data *data) {
    int top = data->in.top;
    int state;

    if (top < 1) return;

    stralloc_0( &data->cdata );
    state = calc_state(data->in.st[ top - 1 ], data->in.st[ top ]);

    switch(state) {
        struct tm tm;
        case STATE_sitename:
            stralloc_copy(&data->siteinfo.sitename, &data->cdata);
            break;
        case STATE_sitebase:
            stralloc_copy(&data->siteinfo.domain, &data->cdata);
            break;
        case STATE_namespace:
            stralloc_copy(&data->siteinfo.namespaces[data->siteinfo.nr_ns], &data->cdata);
            data->siteinfo.nr_ns++;
            break;

        case STATE_pageid:
            data->page.id = atoi( data->cdata.s );
            break;
        case STATE_title:
            stralloc_copy(&data->page.title, &data->cdata);
            break;

        case STATE_revid:
            data->rev.id = atoi( data->cdata.s );
            break;
        case STATE_comment:
            stralloc_copy(&data->rev.comment, &data->cdata);
            break;
        case STATE_text:
            stralloc_copy(&data->rev.text, &data->cdata);
        case STATE_revtime:
            byte_zero(&tm, sizeof(struct tm));
            strptime(data->cdata.s, "%Y-%m-%dT%H:%M:%SZ", &tm);
            tai_unix(&data->rev.timestamp, mktime(&tm));
            break;

    }

}

void output_revision(mw_data *data) {
    size_t len;
    stralloc out;
    char buf[FMT_LONG];

    stralloc_init(&out);
    stralloc_cats(&out, "blob\ndata ");
    len = fmt_long(buf, data->rev.text.len);
    stralloc_catb(&out, buf, len);
    stralloc_cats(&out, "\n");
    stralloc_cat(&out, &data->rev.text);
    stralloc_cats(&out, "\n");


    buffer_putsa(STDOUT, &out);
    buffer_flush(STDOUT);

    stralloc_free(&out);
}
int tagsearch(struct tagmap* map, char* tag, int size) {
    int cond;
    int low, high, mid;

    low = 0;
    high = size - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        cond = str_diff(tag, map[mid].tag);
        if (cond < 0)       high = mid - 1;
        else if (cond > 0)  low = mid + 1;
        else                return mid;
    }
    return -1;
}

#define IO_BUF_SIZE 4096
int main() {
    mw_data user_data;
    data_init(&user_data);
    iksparser *prs;
    char buf[IO_BUF_SIZE];
    int len;

    buffer_putsflush(STDOUT, "progress start\n");

    prs = iks_sax_new(&user_data, taghook, cdatahook);

    io_fd(0);
    while ( (len = io_waitread(0, buf, IO_BUF_SIZE)) > 0 ) {
        switch(iks_parse( prs, buf, len, 1 )) {
            case IKS_OK:
                break;
            case IKS_NOMEM:
                buffer_puts(STDERR, "Not enough memory\n");
            case IKS_BADXML:
                buffer_puts(STDERR, "XML document is not well-formed\n");
            case IKS_HOOK:
                buffer_puts(STDERR, "Our hooks didn't like something\n");
        }
    }
    buffer_puts(STDERR, "Bytes: ");
    buffer_putlong(STDERR, iks_nr_bytes(prs));
    buffer_putnlflush(STDERR);
    buffer_puts(STDERR, "Lines: ");
    buffer_putlong(STDERR, iks_nr_lines(prs));
    buffer_putnlflush(STDERR);
    iks_parser_delete (prs);
    return 0;
}
