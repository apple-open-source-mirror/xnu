/*
 * Copyright (c) 2011-2012 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 *
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 *
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

#ifndef _NET_CLASSQ_CLASSQ_SFB_H_
#define	_NET_CLASSQ_CLASSQ_SFB_H_

#ifdef PRIVATE
#ifdef BSD_KERNEL_PRIVATE
#include <net/classq/if_classq.h>
#include <stdbool.h>
#include <sys/time.h>
#endif /* BSD_KERNEL_PRIVATE */

#ifdef __cplusplus
extern "C" {
#endif

#define	SFB_FP_SHIFT	14			/* fixed-point shift (Q14) */
#define	SFB_LEVELS	2			/* L */
#define	SFB_BINS_SHIFT	5
#define	SFB_BINS	(1 << SFB_BINS_SHIFT)	/* N */

struct sfbstats {
	u_int64_t		drop_early;
	u_int64_t		drop_pbox;
	u_int64_t		drop_queue;
	u_int64_t		marked_packets;
	u_int64_t		pbox_packets;
	u_int64_t		pbox_time;
	u_int64_t		hold_time;
	u_int64_t		dequeue_avg;
	u_int64_t		rehash_intval;
	u_int64_t		num_rehash;
	u_int64_t		null_flowhash;
	u_int64_t		flow_controlled;
	u_int64_t		flow_feedback;
};

struct sfbbinstats {
	int16_t		pmark;		/* marking probability in Q format */
	u_int16_t	pkts;		/* number of packets */
};

struct sfb_stats {
	u_int32_t		allocation;
	u_int32_t		dropthresh;
	u_int32_t		clearpkts;
	u_int32_t		current;
	struct sfbstats		sfbstats;
	struct sfbbins {
		struct sfbbinstats stats[SFB_LEVELS][SFB_BINS];
	} binstats[2] __attribute__((aligned(8)));
};

#ifdef BSD_KERNEL_PRIVATE
struct sfb_bin_fcentry {
	SLIST_ENTRY(sfb_bin_fcentry) fce_link;
	u_int32_t	fce_flowhash;
};

SLIST_HEAD(sfb_fc_list, sfb_bin_fcentry);

struct sfb_bins {
	u_int32_t		fudge;
	struct sfbbinstats	stats[SFB_LEVELS][SFB_BINS];
	struct timespec		freezetime[SFB_LEVELS][SFB_BINS];
};

/* SFB flags */
#define	SFBF_ECN4	0x01	/* use packet marking for IPv4 packets */
#define	SFBF_ECN6	0x02	/* use packet marking for IPv6 packets */
#define	SFBF_ECN	(SFBF_ECN4 | SFBF_ECN6)
#define	SFBF_FLOWCTL	0x04	/* enable flow control advisories */
#define	SFBF_SUSPENDED	0x1000	/* queue is suspended */

#define	SFBF_USERFLAGS							\
	(SFBF_ECN4 | SFBF_ECN6 | SFBF_FLOWCTL)

typedef struct sfb {
	/* variables for internal use */
	u_int32_t	sfb_flags;	/* SFB flags */
	u_int32_t	sfb_qlim;
	u_int32_t	sfb_qid;
	u_int16_t	sfb_allocation;
	u_int16_t	sfb_drop_thresh;
	u_int32_t	sfb_clearpkts;
	u_int64_t	sfb_eff_rate;	/* last known effective rate */
	struct timespec	sfb_getqtime;	/* last dequeue timestamp */
	struct timespec	sfb_holdtime;	/* random holdtime in nsec */
	struct ifnet	*sfb_ifp;	/* back pointer to ifnet */

	/* moving hash function */
	struct timespec	sfb_hinterval;	/* random reset interval in sec */
	struct timespec	sfb_nextreset;	/* reset deadline */

	/* penalty box */
	struct timespec	sfb_pboxtime;	/* random pboxtime in nsec */
	struct timespec	sfb_pboxfreeze;

	/* B[L][N] bins (2 sets: current and warm-up) */
	u_int32_t	sfb_current;	/* current set (0 or 1) */
	struct sfb_bins	(*sfb_bins)[2];

	/* Flow control lists for current set */
	struct sfb_fc_list (*sfb_fc_lists)[SFB_BINS];

	/* statistics */
	struct sfbstats	sfb_stats __attribute__((aligned(8)));
} sfb_t;

extern void sfb_init(void);
extern struct sfb *sfb_alloc(struct ifnet *, u_int32_t, u_int32_t, u_int32_t);
extern void sfb_destroy(struct sfb *);
extern int sfb_addq(struct sfb *, class_queue_t *, struct mbuf *,
    struct pf_mtag *);
extern struct mbuf *sfb_getq(struct sfb *, class_queue_t *);
extern void sfb_purgeq(struct sfb *, class_queue_t *, u_int32_t,
    u_int32_t *, u_int32_t *);
extern void sfb_getstats(struct sfb *, struct sfb_stats *);
extern void sfb_updateq(struct sfb *, cqev_t);
extern int sfb_suspendq(struct sfb *, class_queue_t *, boolean_t);
#endif /* BSD_KERNEL_PRIVATE */

#ifdef __cplusplus
}
#endif
#endif /* PRIVATE */
#endif /* _NET_CLASSQ_CLASSQ_SFB_H_ */
