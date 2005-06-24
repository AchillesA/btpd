#ifndef BTPD_H
#define BTPD_H

#include <sys/types.h>
#include <sys/time.h>

#include <assert.h>
#include <errno.h>
#include <event.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

#include "queue.h"

#include "benc.h"
#include "metainfo.h"
#include "iobuf.h"
#include "net.h"
#include "peer.h"
#include "torrent.h"
#include "policy.h"
#include "subr.h"

#define BTPD_VERSION (PACKAGE_NAME "/" PACKAGE_VERSION)

struct child {
    pid_t pid;
    void (*child_done)(struct child *child);
    TAILQ_ENTRY(child) entry;
};

TAILQ_HEAD(child_tq, child);

struct btpd {
    uint8_t peer_id[20];

    const char *version;

    uint32_t logmask;

    struct child_tq kids;

    unsigned ntorrents;
    struct torrent_tq cm_list;

    struct peer_tq readq;
    struct peer_tq writeq;

    int port;
    int peer4_sd;
    int ipc_sd;

    unsigned long obwlim, ibwlim;
    unsigned long ibw_left, obw_left;
    
    unsigned npeers;
    unsigned maxpeers;

    unsigned long seconds;

    struct event cli;
    struct event accept4;

    struct event heartbeat;

    struct event sigint;
    struct event sigterm;
    struct event sigchld;
};

extern struct btpd btpd;

#define BTPD_L_ALL	0xffffffff
#define BTPD_L_ERROR	0x00000001
#define BTPD_L_TRACKER	0x00000002
#define BTPD_L_CONN	0x00000004
#define BTPD_L_MSG	0x00000008
#define BTPD_L_BTPD	0x00000010
#define BTPD_L_POL	0x00000020

void btpd_log(uint32_t type, const char *fmt, ...);

void btpd_err(const char *fmt, ...);

void *btpd_malloc(size_t size);
void *btpd_calloc(size_t nmemb, size_t size);

void btpd_shutdown(void);

#endif