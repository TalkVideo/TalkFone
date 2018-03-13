 /*
 * libiax: An implementation of Inter-Asterisk eXchange
 *
 * Copyright (C) 2001, Linux Support Services, Inc.
 *
 * Mark Spencer <markster@linux-support.net>
 * Frik Strecker <frik@gatherworks.com>
 *
 * This program is free software, distributed under the terms of
 * the GNU Lesser (Library) General Public License
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(WIN32)  ||  defined(_WIN32_WCE)
#undef __STRICT_ANSI__ //for strdup with ms

#if defined(_WIN32_WCE)
#define strdup _strdup
#else
#include <process.h>
#include <fcntl.h>
#include <io.h>
#include <errno.h>
#endif
#include <string.h>
#include <windows.h>
#include <winsock.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

#if !defined(_WIN32_WCE)
#include <sys/timeb.h>
#endif

#define	snprintf _snprintf

#if defined(_MSC_VER)
#define	close		closesocket
#if !defined(_WIN32_WCE)
#define inline      __inline
#endif
#endif

#else

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <stdlib.h>
#ifdef __GNUC__
#ifndef __USE_SVID
#define __USE_SVID
#endif
#endif
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#if !defined(MACOSX) && !defined(__OpenBSD__)
#include <malloc.h>
#if !defined(SOLARIS)
#include <error.h>
#endif
#endif

#endif

#include "jitterbuf.h"
#include "iax-client.h"
#include "md5.h"

/* Define socket options for IAX2 sockets, based on platform
 * availability of flags */
#if defined(WIN32)  ||  defined(_WIN32_WCE)
#define IAX_SOCKOPTS 0
#else
#ifdef MACOSX
#define IAX_SOCKOPTS MSG_DONTWAIT
#else
#if defined(SOLARIS) || defined(__OpenBSD__)
#define IAX_SOCKOPTS MSG_DONTWAIT
#else  /* Linux and others */
#define IAX_SOCKOPTS MSG_DONTWAIT | MSG_NOSIGNAL
#endif
#endif
#endif


#ifdef SNOM_HACK
/* The snom phone seems to improperly execute memset in some cases */
#include "../../snom_phonecore2/include/snom_memset.h"
#endif

/* Voice TS Prediction causes libiax2 to clean up the timestamps on
 * outgoing frames.  It works best with either continuous voice, or
 * callers who call iax_send_cng to indicate DTX for silence */
#define USE_VOICE_TS_PREDICTION

#define MIN_RETRY_TIME 10
#define MAX_RETRY_TIME 4000
#define MEMORY_SIZE 1000

#define TRANSFER_NONE  0
#define TRANSFER_BEGIN 1
#define TRANSFER_READY 2
#define TRANSFER_REL   3

/* Video frames bypass jitterbuffer */
static int video_bypass_jitterbuffer = 0;

/* To use or not to use the jitterbuffer */
static int iax_use_jitterbuffer = 1;

/* UDP Socket (file descriptor) */
static int netfd = -1;

/* Max timeouts */
static const int maxretries = 10;

/* configurable jitterbuffer options */
static long jb_target_extra = -1;

/* external global networking replacements */
static iax_sendto_t   iax_sendto = (iax_sendto_t) sendto;
static iax_recvfrom_t iax_recvfrom = (iax_recvfrom_t) recvfrom;

/* ping interval (seconds) */
static int ping_time = 10;
static void send_ping(void *session);

struct iax_session {
	/* Private data */
	void *pvt;
	/* session-local Sendto function */
	iax_sendto_t sendto;
	/* Is voice quelched (e.g. hold) */
	int quelch;
	/* Codec Pref Order */
	char codec_order[32];
	/* Codec Pref Order Index*/
	int codec_order_len;
	/* Last received voice format */
	int voiceformat;
	/* Last transmitted voice format */
	int svoiceformat;
	/* Last received video format */
	int videoformat;
	/* Last transmitted video format */
	int svideoformat;
	/* Per session capability */
	int capability;
	/* Last received timestamp */
	unsigned int last_ts;
	/* Last transmitted timestamp */
	unsigned int lastsent;
	/* Timestamp of the last transmitted video frame */
	unsigned int lastvsent;
#ifdef USE_VOICE_TS_PREDICTION
	/* Next predicted voice ts */
	unsigned int nextpred;
	/* True if the last voice we transmitted was not silence/CNG */
	int notsilenttx;
#endif
	/* Our last measured ping time */
	unsigned int pingtime;
	/* Address of peer */
	struct sockaddr_in peeraddr;
	/* Our call number */
	int callno;
	/* Peer's call number */
	int peercallno;
	/* Our next outgoing sequence number */
	unsigned char oseqno;
	/* Next sequence number they have not yet acknowledged */
	unsigned char rseqno;
	/* Our last received incoming sequence number */
	unsigned char iseqno;
	/* Last acknowledged sequence number */
	unsigned char aseqno;
	/* Last sequence number we VNAKd */
	unsigned char lastvnak;
	/* Time value that we base our transmission on */
	struct timeval offset;
	/* Time value we base our delivery on */
	struct timeval rxcore;
	/* Current link state */
	int state;
	/* Unregister reason */
	char unregreason[MAXSTRLEN];
	/* Expected Username */
	char username[MAXSTRLEN];
	/* Expected Secret */
	char secret[MAXSTRLEN];
	/* Refresh if applicable */
	int refresh;

	/* ping scheduler id */
	int pingid;

	/* Transfer stuff */
	struct sockaddr_in transfer;
	int transferring;
	int transfercallno;
	int transferid;
	int transferpeer;	/* for attended transfer */
	int transfer_moh;	/* for music on hold while performing attended transfer */

	jitterbuf *jb;

	struct iax_netstat remote_netstats;

	/* For linking if there are multiple connections */
	struct iax_session *next;
};

char iax_errstr[256];


#define IAXERROR snprintf(iax_errstr, sizeof(iax_errstr),

#ifdef DEBUG_SUPPORT

#ifdef DEBUG_DEFAULT
static int debug = 1;
#else
static int debug = 0;
#endif

void iax_enable_debug(void)
{
	debug = 1;
}

void iax_disable_debug(void)
{
	debug = 0;
}

void iax_enable_jitterbuffer(void)
{
	iax_use_jitterbuffer = 1;
}

void iax_disable_jitterbuffer(void)
{
	iax_use_jitterbuffer = 0;
}

void iax_set_private(struct iax_session *s, void *ptr)
{
	s->pvt = ptr;
}

void *iax_get_private(struct iax_session *s)
{
	return s->pvt;
}

void iax_set_sendto(struct iax_session *s, iax_sendto_t ptr)
{
	s->sendto = ptr;
}

/* This is a little strange, but to debug you call DEBU(G "Hello World!\n"); */
#if defined(WIN32)  ||  defined(_WIN32_WCE)
#define G __FILE__, __LINE__,
#else
#define G __FILE__, __LINE__, __PRETTY_FUNCTION__,
#endif

#define DEBU __debug
#if defined(WIN32)  ||  defined(_WIN32_WCE)
static int __debug(const char *file, int lineno, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	if (debug) {
		fprintf(stderr, "%s line %d: ", file, lineno);
		vfprintf(stderr, fmt, args);
	}
	va_end(args);
	return 0;
}
#else
static int __debug(const char *file, int lineno, const char *func, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	if (debug) {
		fprintf(stderr, "%s line %d in %s: ", file, lineno, func);
		vfprintf(stderr, fmt, args);
	}
	va_end(args);
	return 0;
}
#endif
#else /* No debug support */

#if defined(WIN32)  ||  defined(_WIN32_WCE)
#define	DEBU
#else
#define DEBU(fmt...) \
    do {} while(0)
#endif
#define G
#endif

void iax_seed_random()
{
#if defined(HAVE_SRANDOMDEV)
	srandomdev();
#elif defined(HAVE_SRANDOM)
	srandom((unsigned int)time(0));
#elif defined(HAVE_SRAND48)
	srand48((long int)time(0));
#else
	srand((unsigned int)time(0));
#endif
}

int iax_random()
{
#if defined(HAVE_RANDOM)
	return (int)random();
#elif defined(HAVE_LRAND48)
	return (int)lrand48();
#else
	return rand();
#endif
}

typedef void (*sched_func)(void *);

struct iax_sched {
	/* These are scheduled things to be delivered */
	struct timeval when;
	/* If event is non-NULL then we're delivering an event */
	struct iax_event *event;
	/* If frame is non-NULL then we're transmitting a frame */
	struct iax_frame *frame;
	/* If func is non-NULL then we should call it */
	sched_func func;
	/* and pass it this argument */
	void *arg;
	/* Easy linking */
	struct iax_sched *next;
};

static struct iax_sched *schedq = NULL;
static struct iax_session *sessions = NULL;
static int callnums = 1;

static inline struct timeval iax_tvadd_ms(struct timeval tv, int ms)
{
	const int million = 1000000;
	tv.tv_sec += ms / 1000;
	ms %= 1000;
	tv.tv_usec += ms * 1000;
	if ( tv.tv_usec > million )
	{
		tv.tv_sec += 1;
		tv.tv_usec -= million;
	}
	return tv;
}

static inline int iax_tvdiff_ms(struct timeval end, struct timeval start)
{
	return ((end.tv_sec - start.tv_sec) * 1000) +
		(((1000000 + end.tv_usec - start.tv_usec) / 1000) - 1000);
}

static inline int iax_tvcmp(struct timeval a, struct timeval b)
{
	if ( a.tv_sec < b.tv_sec )
		return -1;
	if ( a.tv_sec > b.tv_sec )
		return 1;
	if ( a.tv_usec < b.tv_usec )
		return -1;
	if ( a.tv_usec > b.tv_usec )
		return 1;
	return 0;
}

static inline int iax_tvzero(const struct timeval t)
{
	return t.tv_sec == 0 && t.tv_usec == 0;
}

unsigned int iax_session_get_capability(struct iax_session *s)
{
	return s->capability;
}

static int inaddrcmp(struct sockaddr_in *sin1, struct sockaddr_in *sin2)
{
	return (sin1->sin_addr.s_addr != sin2->sin_addr.s_addr) || (sin1->sin_port != sin2->sin_port);
}

static int iax_sched_add(struct iax_event *event, struct iax_frame *frame, sched_func func, void *arg, int ms)
{
	/* Schedule event to be delivered to the client in ms milliseconds from
	 * now, or a reliable frame to be retransmitted */
	struct iax_sched *sched;
	struct iax_sched *cur = schedq;
	struct iax_sched *prev = NULL;

	if (!event && !frame && !func) {
		DEBU(G "No event, no frame, no func?  what are we scheduling?\n");
		return -1;
	}

	sched = calloc(1, sizeof(struct iax_sched));
	if (!sched) {
		DEBU(G "Out of memory!\n");
		return -1;
	}

	sched->when = iax_tvadd_ms(iax_tvnow(), ms);
	sched->event = event;
	sched->frame = frame;
	sched->func = func;
	sched->arg = arg;

	/* Put it in the list, in order */
	while (cur && iax_tvcmp(cur->when, sched->when) <= 0) {
		prev = cur;
		cur = cur->next;
	}
	sched->next = cur;
	if (prev) {
		prev->next = sched;
	} else {
		schedq = sched;
	}

	return 0;
}

static int iax_sched_del(struct iax_event *event, struct iax_frame *frame, sched_func func, void *arg, int all)
{
	struct iax_sched *cur = schedq;
	struct iax_sched *prev = NULL;

	while (cur) {
		if (cur->event == event && cur->frame == frame && cur->func == func && cur->arg == arg) {
			struct iax_sched *tmp = cur;
			if (prev)
				prev->next = cur->next;
			else
				schedq = cur->next;
			cur = cur->next;
			free(tmp);
			if (!all)
				return -1;
		} else {
			prev = cur;
			cur = cur->next;
		}
	}
	return 0;
}

int iax_time_to_next_event(void)
{
	struct timeval now;
	struct iax_sched *cur = schedq;
	int min = 999999999;

	/* If there are no pending events, we don't need to timeout */
	if (!cur)
		return -1;

	now = iax_tvnow();

	while(cur) {
		int ms = iax_tvdiff_ms(cur->when, now);
		if (ms < min)
			min = ms;
		cur = cur->next;
	}
	return min < 0 ? 0 : min;
}

struct iax_session *iax_session_new(void)
{
	struct iax_session *s;
	s = calloc(1, sizeof(struct iax_session));
	if (s) {
		jb_conf jbconf;

		/* Initialize important fields */
		s->voiceformat = -1;
		s->svoiceformat = -1;
		s->videoformat = -1;
		/* Default pingtime to 100 ms -- should cover most decent net connections */
		s->pingtime = 100;
		/* XXX Not quite right -- make sure it's not in use, but that won't matter
		   unless you've had at least 65k calls.  XXX */
		s->callno = callnums++;
		if (callnums > 32767)
			callnums = 1;
		s->peercallno = 0;
		s->lastvnak = -1;
		s->transferpeer = 0; /* for attended transfer */
		s->next = sessions;
		s->sendto = iax_sendto;
		s->pingid = -1;

#ifdef USE_VOICE_TS_PREDICTION
		s->nextpred = 0;
#endif

		s->jb = jb_new();
		if ( !s->jb )
		{
			free(s);
			return 0;
		}
		jbconf.max_jitterbuf = 0;
		jbconf.resync_threshold = 1000;
		jbconf.max_contig_interp = 0;
		jbconf.target_extra = jb_target_extra;
		jb_setconf(s->jb, &jbconf);

		sessions = s;
	}
	return s;
}

static int iax_session_valid(struct iax_session *session)
{
	/* Return -1 on a valid iax session pointer, 0 on a failure */
	struct iax_session *cur = sessions;
	while(cur) {
		if (session == cur)
			return -1;
		cur = cur->next;
	}
	return 0;
}

int iax_get_netstats(struct iax_session *session, int *rtt, struct iax_netstat *local, struct iax_netstat *remote)
{
	jb_info stats;

	if(!iax_session_valid(session)) return -1;

	*rtt = session->pingtime;

	*remote = session->remote_netstats;

	jb_getinfo(session->jb, &stats);

	local->jitter = stats.jitter;
	/* XXX: should be short-term loss pct.. */
	if(stats.frames_in == 0)
		stats.frames_in = 1;
	local->losspct = stats.losspct/1000;
	local->losscnt = stats.frames_lost;
	local->packets = stats.frames_in;
	local->delay = stats.current - stats.min;
	local->dropped = stats.frames_dropped;
	local->ooo = stats.frames_ooo;

	return 0;
}

static int calc_timestamp(struct iax_session *session, int frametype, int samples)
{
	/* Calculate the number of milliseconds since we sent the first packet */
	int ms = iax_tvdiff_ms(iax_tvnow(), session->offset);
	int genuine = !frametype || frametype == AST_FRAME_IAX;
	int voice = frametype == AST_FRAME_VOICE;
	int video = frametype == AST_FRAME_VIDEO;

	if (ms < 0) {
		fprintf(stderr, "iax2: calc_timestamp: negative time delta "
				"ms => %d\n", ms);
		ms = 0;
	}

	if (voice) {
#ifdef USE_VOICE_TS_PREDICTION
		/* If we haven't most recently sent silence, and we're
		 * close in time, use predicted time */
		if(session->notsilenttx && abs(ms - session->nextpred) <= 240) {
			/* Adjust our txcore, keeping voice and non-voice
			 * synchronized */
			session->offset = iax_tvadd_ms(session->offset,
					(int)(ms - session->nextpred) / 10);

			if(!session->nextpred)
				session->nextpred = ms;
			ms = session->nextpred;
		} else {
			/* in this case, just use the actual time, since
			 * we're either way off (shouldn't happen), or we're
			 * ending a silent period -- and seed the next predicted
			 * time.  Also, round ms to the next multiple of
			 * frame size (so our silent periods are multiples
			 * of frame size too) */
			int diff = ms % (samples / 8);
			if(diff)
				ms += samples / 8 - diff;
			session->nextpred = ms;
		}
		session->notsilenttx = 1;

		/* set next predicted ts based on 8khz samples */
		session->nextpred += samples / 8;
#else
		if(ms <= session->lastsent)
			ms = session->lastsent + 3;
#endif
	} else if ( video ) {
		/* IAX2 draft 03 says that timestamps MUST be in order.  It
		 * does not say anything about several frames having the same
		 * timestamp.  When transporting video, we can have a frame
		 * that spans multiple iax packets (so called slices), so it
		 * would make sense to use the same timestamp for all of them.
		 * We do want to make sure that frames don't go backwards
		 * though.
		 */
		if ( (unsigned int)ms < session->lastsent )
			ms = session->lastsent;
	} else {
		/* On a dataframe, use last value + 3 (to accomodate jitter
		 * buffer shrinking) if appropriate unless it's a genuine frame
		 */
		if (genuine) {
			if ((unsigned int)ms <= session->lastsent)
				ms = session->lastsent + 3;
		} else if (abs(ms - session->lastsent) <= 240) {
			ms = session->lastsent + 3;
		}

	}

	return ms;
}

static unsigned char get_n_bits_at(const unsigned char *data, const int n, const int bit)
{
	int byte = bit / 8;       /* byte containing first bit */
	int rem = 8 - (bit % 8);  /* remaining bits in first byte */
	unsigned char ret = 0;

	if (n <= 0 || n > 8)
		return 0;

	if (rem < n) {
		ret = (data[byte] << (n - rem));
		ret |= (data[byte + 1] >> (8 - n + rem));
	} else {
		ret = (data[byte] >> (rem - n));
	}

	return (ret & (0xff >> (8 - n)));
}

static int speex_get_wb_sz_at(const unsigned char *data, const int len, const int bit)
{
	static const int wb_skip_table[] = {
		0, 36, 112, 192, 352, 0, 0, 0
	};
	const int bit_len = len * 8;
	int off = bit;
	int wideband;

	if (bit_len - off < 5)
		return -5;

	wideband = get_n_bits_at(data, 1, off);

	/* skip up to two wideband frames */
	if (wideband)
	{
		int submode = get_n_bits_at(data, 3, off + 1);
		off += wb_skip_table[submode];

		if (bit_len - off < 5)
			return -6;

		wideband = get_n_bits_at(data, 1, off);

		if (wideband)
		{
			submode = get_n_bits_at(data, 3, off + 1);
			off += wb_skip_table[submode];

			if (bit_len - off < 5)
				return -7;

			wideband = get_n_bits_at(data, 1, off);
			if (wideband)
				/* too many in a row */
				return -8;
		}

	}
	return off - bit;
}

static int speex_get_samples(const unsigned char *data, const int len)
{
	static const int SpeexSubModeSz[] = {
		0, 43, 119, 160,
		220, 300, 364, 492,
		79, 0, 0, 0,
		0, 0, 0, 0 };
	static const int SpeexInBandSz[] = {
		1, 1, 4, 4,
		4, 4, 4, 4,
		8, 8, 16, 16,
		32, 32, 64, 64 };
	const int bit_len = len * 8;
	int bit = 0;
	int cnt = 0;

	while (bit_len - bit >= 5) {
		unsigned char c;

		/* skip wideband frames */
		const int advance = speex_get_wb_sz_at(data, len, bit);
		if (advance < 0)
			return advance;

		bit += advance;

		if (bit_len - bit < 5)
			return -1;

		/* get control bits */
		c = get_n_bits_at(data, 5, bit);
		bit += 5;

		if (c == 15) {
			/* terminator */
			break;
		} else if (c == 14) {
			/* in-band signal; next 4 bits contain signal id */
			if (bit_len - bit < 4)
				return -3;
			c = get_n_bits_at(data, 4, bit);
			bit += 4 + SpeexInBandSz[c];
		} else if (c == 13) {
			/* user in-band; next 5 bits contain msg len */
			if (bit_len - bit < 5)
				return -4;
			c = get_n_bits_at(data, 5, bit);
			bit += 5 + c * 8;
		} else if (c > 8) {
			/* unknown submode */
			return -2;
		} else {
			/* skip number bits for submode (less the 5 control bits) */
			bit += SpeexSubModeSz[c] - 5;
			cnt += 160; /* new frame */
		}
	}
	return cnt;
}

static inline int get_interp_len(int format)
{
	return (format == AST_FORMAT_ILBC) ? 30 : 20;
}

static int get_sample_cnt(struct iax_event *e)
{
	int cnt = 0;

	/*
	 * In the case of zero length frames, do not return a cnt of 0
	 */
	if ( e->datalen == 0 ) {
		return get_interp_len( e->subclass ) * 8;
	}

	switch (e->subclass) {
	case AST_FORMAT_SPEEX:
		cnt = speex_get_samples(e->data, e->datalen);
		break;
	case AST_FORMAT_G723_1:
		cnt = 240;		/* FIXME Not always the case */
		break;
	case AST_FORMAT_ILBC:
		cnt = 240 * (e->datalen / 50);
		break;
	case AST_FORMAT_GSM:
		cnt = 160 * (e->datalen / 33);
		break;
	case AST_FORMAT_G729A:
		cnt = 160 * (e->datalen / 20);
		break;
	case AST_FORMAT_SLINEAR:
		cnt = e->datalen / 2;
		break;
	case AST_FORMAT_LPC10:
		cnt = 22 * 8 + (((char *)(e->data))[7] & 0x1) * 8;
		break;
	case AST_FORMAT_ULAW:
	case AST_FORMAT_ALAW:
		cnt = e->datalen;
		break;
	case AST_FORMAT_ADPCM:
	case AST_FORMAT_G726:
		cnt = e->datalen * 2;
		break;
	default:
		return 0;
	}
	return cnt;
}

static int iax_xmit_frame(struct iax_frame *f)
{
	int res;
#ifdef DEBUG_SUPPORT
	if (debug) {
		struct ast_iax2_full_hdr *h = (struct ast_iax2_full_hdr *)f->data;

		if (ntohs(h->scallno) & IAX_FLAG_FULL)
			iax_showframe(f, NULL, 0, f->transfer ?
					&(f->session->transfer) :
					&(f->session->peeraddr),
					f->datalen - sizeof(struct ast_iax2_full_hdr));
	}
#endif
	/* Send the frame raw */
	res = f->session->sendto(netfd, f->rawdata, f->rawdatalen,
			IAX_SOCKOPTS, f->transfer ?
			(struct sockaddr *)&(f->session->transfer) :
			(struct sockaddr *)&(f->session->peeraddr),
			sizeof(f->session->peeraddr));
	return res;
}

static int iax_reliable_xmit(struct iax_frame *f)
{
	struct ast_iax2_full_hdr *fh = f->rawdata;

	if (!fh->type)
		return -2;

	iax_sched_add(NULL, f, NULL, NULL, f->retrytime);
	return iax_xmit_frame(f);
}

void iax_set_networking(iax_sendto_t st, iax_recvfrom_t rf)
{
	iax_sendto = st;
	iax_recvfrom = rf;
}

void iax_set_jb_target_extra( long value )
{
	/* store in jb_target_extra, a static global */
	jb_target_extra = value ;
}

int iax_init(int preferredportno)
{
	int portno = preferredportno;

	if (iax_recvfrom == (iax_recvfrom_t)recvfrom)
	{
		struct sockaddr_in sin;
		socklen_t sinlen;
		int flags;
		int bufsize = 128 * 1024;

		if (netfd > -1)
		{
			/* Okay, just don't do anything */
			DEBU(G "Already initialized.");
			return 0;
		}
		netfd = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		if (netfd < 0)
		{
			DEBU(G "Unable to allocate UDP socket\n");
			IAXERROR "Unable to allocate UDP socket\n");
			return -1;
		}

		if (preferredportno == 0)
			preferredportno = IAX_DEFAULT_PORTNO;

		if (preferredportno < 0)
			preferredportno = 0;

		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = 0;
		sin.sin_port = htons((short)preferredportno);
		if (bind(netfd, (struct sockaddr *) &sin, sizeof(sin)) < 0)
		{
#if defined(WIN32)  ||  defined(_WIN32_WCE)
			if (WSAGetLastError() == WSAEADDRINUSE)
#else
			if (errno == EADDRINUSE)
#endif
			{
				/*the port is already in use, so bind to a free port chosen by the IP stack*/
				DEBU(G "Unable to bind to preferred port - port is in use. Trying to bind to a free one");
				sin.sin_port = htons((short)0);
				if (bind(netfd, (struct sockaddr *) &sin, sizeof(sin)) < 0)
				{
					IAXERROR "Unable to bind UDP socket\n");
					return -1;
				}
			} else
			{
				IAXERROR "Unable to bind UDP socket\n");
				return -1;
			}
		}

		sinlen = sizeof(sin);
		if (getsockname(netfd, (struct sockaddr *) &sin, &sinlen) < 0)
		{
			close(netfd);
			netfd = -1;
			DEBU(G "Unable to figure out what I'm bound to.");
			IAXERROR "Unable to determine bound port number.");
			return -1;
		}
#if defined(WIN32)  ||  defined(_WIN32_WCE)
		flags = 1;
		if (ioctlsocket(netfd,FIONBIO,(unsigned long *) &flags))
		{
			closesocket(netfd);
			netfd = -1;
			DEBU(G "Unable to set non-blocking mode.");
			IAXERROR "Unable to set non-blocking mode.");
			return -1;
		}

#else
		if ((flags = fcntl(netfd, F_GETFL)) < 0)
		{
			close(netfd);
			netfd = -1;
			DEBU(G "Unable to retrieve socket flags.");
			IAXERROR "Unable to retrieve socket flags.");
			return -1;
		}
		if (fcntl(netfd, F_SETFL, flags | O_NONBLOCK) < 0)
		{
			close(netfd);
			netfd = -1;
			DEBU(G "Unable to set non-blocking mode.");
			IAXERROR "Unable to set non-blocking mode.");
			return -1;
		}
#endif
		/* Mihai: increase UDP socket buffers to avoid packet loss. */
		if (setsockopt(netfd, SOL_SOCKET, SO_RCVBUF, (char *)&bufsize,
					sizeof(bufsize)) < 0)
		{
			DEBU(G "Unable to set receive buffer size.");
			IAXERROR "Unable to set receive buffer size.");
		}

		/* set send buffer size too */
		if (setsockopt(netfd, SOL_SOCKET, SO_SNDBUF, (char *)&bufsize,
					sizeof(bufsize)) < 0)
		{
			DEBU(G "Unable to set send buffer size.");
			IAXERROR "Unable to set send buffer size.");
		}

		portno = ntohs(sin.sin_port);
		DEBU(G "Started on port %d\n", portno);
	}

	iax_seed_random();
	callnums = 1 + (int)(32767.0 * (iax_random() / (RAND_MAX + 1.0)));

	return portno;
}

static void destroy_session(struct iax_session *session);

static void convert_reply(char *out, unsigned char *in)
{
	int x;
	for (x=0;x<16;x++)
		sprintf(out + (x << 1), "%2.2x", (int)in[x]);
}

static unsigned char compress_subclass(int subclass)
{
	int x;
	int power=-1;
	/* If it's 128 or smaller, just return it */
	if (subclass < IAX_FLAG_SC_LOG)
		return subclass;
	/* Otherwise find its power */
	for (x = 0; x < IAX_MAX_SHIFT; x++) {
		if (subclass & (1 << x)) {
			if (power > -1) {
				DEBU(G "Can't compress subclass %d\n", subclass);
				return 0;
			} else
				power = x;
		}
	}
	return power | IAX_FLAG_SC_LOG;
}

static int iax_send(struct iax_session *pvt,
		int frametype,
		int subclass,
		const unsigned char *data,
		const size_t datalen,
		unsigned int ts,
		int seqno,
		int immediate,
		int transfer,
		int final,
		int fullframe,
		int samples)
{
	/* Queue a packet for delivery on a given private structure.  Use "ts"
	 * for timestamp, or calculate if ts is 0.  Send immediately without
	 * retransmission or delayed, with retransmission */
	char buf[8192];
	struct iax_frame *fr;
	int sendmini = 0;

	if (!pvt)
	{
		IAXERROR "No private structure for packet?\n");
		return -1;
	}

	/* If this is the first packet we're sending, get our offset now. */
	if (iax_tvzero(pvt->offset))
		pvt->offset = iax_tvnow();

	if (!ts)
	{
		/* Calculate actual timestamp */
		ts = calc_timestamp(pvt, frametype, samples);

		if (!ts)
		{
			IAXERROR "timestamp is 0?\n");
			return -1;
		}
	}

	if ( frametype == AST_FRAME_VOICE )
	{
		/* High two bits are the same on timestamp, or sending on a trunk */
		if ((ts & 0xFFFF0000L) == (pvt->lastsent & 0xFFFF0000L) &&
				subclass == pvt->svoiceformat)
		{
			/* Force immediate rather than delayed transmission */
			immediate = 1;
			/* Mark that mini-style frame is appropriate */
			sendmini = 1;
		}

		pvt->lastsent = ts;
	}
	else if ( frametype == AST_FRAME_VIDEO )
	{
		/* Check if the timestamp has rolled over or if the video codec
		 * has changed */
		if ( (ts & 0xFFFF8000L) == (pvt->lastvsent & 0xFFFF8000L) &&
		     subclass == pvt->svideoformat )
		{
			/* send a mini video frame immediately */
			immediate = 1;
			sendmini = 1;
		}
		else
		{
			/* we want to send a fullframe and be able to
			 * retransmit it */
			immediate = 0;
			sendmini = 0;
		}

		pvt->lastvsent = ts;
		pvt->lastsent = ts;
	}

	/* if requested, force a full frame */
	if ( fullframe )
	{
		immediate = 0;
		sendmini = 0;
	}

	/* Allocate an iax_frame */
	if (immediate)
	{
		if ( sizeof(*fr) + datalen > sizeof(buf) )
		{
			IAXERROR "frame buffer too small");
			return -1;
		}
		fr = (struct iax_frame *) buf;
	}
	else
	{
		fr = iax_frame_new(datalen);
		if ( fr == NULL )
		{
			IAXERROR "Out of memory\n");
			return -1;
		}
	}

	fr->ts = ts;
	fr->callno = pvt->callno;
	fr->transfer = transfer;
	fr->final = final;
	fr->session = pvt;

	if (!sendmini)
	{
		/* We need a full frame */
		struct ast_iax2_full_hdr *fh;

		iax_frame_specialize(fr, ast_iax2_full_hdr, data, datalen);
		
		fh = fr->rawdata;
		
		fr->oseqno = seqno > -1 ? seqno : pvt->oseqno++;
		fr->iseqno = pvt->iseqno;
		fh->scallno = htons(fr->callno | IAX_FLAG_FULL);
		fh->ts = htonl(fr->ts);
		fh->oseqno = fr->oseqno;
		fh->iseqno = transfer ? 0 : fr->iseqno;
		/* Keep track of the last thing we've acknowledged */
		pvt->aseqno = fr->iseqno;
		fh->type = frametype & 0xFF;
		if (frametype == AST_FRAME_VIDEO)
			fh->csub = compress_subclass(subclass & ~0x1) | ((subclass & 0x1) << 6);
		else
			fh->csub = compress_subclass(subclass);
		fr->dcallno = transfer ? pvt->transfercallno : pvt->peercallno;
		fh->dcallno = htons(fr->dcallno);
		fr->retries = maxretries;
		/* Retry after 2x the ping time has passed */
		fr->retrytime = pvt->pingtime * 2;
		if (fr->retrytime < MIN_RETRY_TIME)
			fr->retrytime = MIN_RETRY_TIME;
		if (fr->retrytime > MAX_RETRY_TIME)
			fr->retrytime = MAX_RETRY_TIME;
		/* Acks' don't get retried */
		if (frametype == AST_FRAME_IAX && subclass == IAX_COMMAND_ACK)
			fr->retries = -1;
		if (frametype == AST_FRAME_VOICE)
			pvt->svoiceformat = subclass;
		else if (frametype == AST_FRAME_VIDEO)
			pvt->svideoformat = subclass & ~0x1;
	}
	else
	{
		/* Mini-frames have no sequence number */
		fr->oseqno = -1;
		fr->iseqno = -1;
		fr->retries = -1;

		if (frametype == AST_FRAME_VIDEO)
		{
			struct ast_iax2_video_hdr *vh;

			iax_frame_specialize(fr, ast_iax2_video_hdr, data, datalen);

			vh = fr->rawdata;
			vh->zeros = 0;
			vh->callno = htons(IAX_FLAG_FULL | fr->callno);
			vh->ts = htons((fr->ts & 0x7FFF) | (subclass & 0x1 ? 0x8000 : 0));
		}
		else
		{
			/* Mini frame will do */
			struct ast_iax2_mini_hdr *mh;

			iax_frame_specialize(fr, ast_iax2_mini_hdr, data, datalen);

			mh = fr->rawdata;
			mh->callno = htons(fr->callno);
			mh->ts = htons(fr->ts & 0xFFFF);
		}
	}

	return immediate ?
		iax_xmit_frame(fr) :
		iax_reliable_xmit(fr);
}

#if 0
static int iax_predestroy(struct iax_session *pvt)
{
	if (!pvt) {
		return -1;
	}
	if (!pvt->alreadygone) {
		/* No more pings or lagrq's */
		if (pvt->pingid > -1)
			ast_sched_del(sched, pvt->pingid);
		if (pvt->lagid > -1)
			ast_sched_del(sched, pvt->lagid);
		if (pvt->autoid > -1)
			ast_sched_del(sched, pvt->autoid);
		if (pvt->initid > -1)
			ast_sched_del(sched, pvt->initid);
		pvt->pingid = -1;
		pvt->lagid = -1;
		pvt->autoid = -1;
		pvt->initid = -1;
		pvt->alreadygone = 1;
	}
	return 0;
}
#endif

static int send_command(struct iax_session *i, char type, int command, unsigned int ts, unsigned char *data, int datalen)
{
	return iax_send(i, type, command, data, datalen, ts,
			-1, /* seqno */
			0, /* immediate */
			0, /* transfer */
			0, /* final */
			0, /* fullframe */ 
			0  /* samples */
			);
}

static int send_command_video(struct iax_session *i, char type, int command, unsigned int ts, unsigned char *data, int datalen, int fullframe)
{
	return iax_send(i, type, command, data, datalen, ts,
			-1, /* seqno */
			1,  /* immediate */
			0,  /* transfer */
			0,  /* final */
			fullframe,
			0); /* samples */
}

static int send_command_final(struct iax_session *i, char type, int command, unsigned int ts, unsigned char *data, int datalen)
{
#if 0
	/* It is assumed that the callno has already been locked */
	iax_predestroy(i);
#endif
	int r = iax_send(i, type, command, data, datalen, ts,
			-1, /* seqno */
			1,  /* immediate */
			0,  /* transfer */
			1,  /* final */
			0,  /* fullframe */
			0); /* samples */
	if (r >= 0)
		destroy_session(i);
	return r;
}

static int send_command_immediate(struct iax_session *i, char type, int command, unsigned int ts, unsigned char *data, int datalen, int seqno)
{
	return iax_send(i, type, command, data, datalen, ts, seqno,
			1,  /* immediate */
			0,  /* transfer */
			0,  /* final */
			0,  /* fullframe */
			0); /* samples */
}

static int send_command_transfer(struct iax_session *i, char type, int command, unsigned char *data, int datalen)
{
	return iax_send(i, type, command, data, datalen,
			0,  /* ts */
			0,  /* seqno */
			0,  /* immediate */
			1,  /* transfer */
			0,  /* final */
			0,  /* fullframe */
			0); /* samples */
}

static int send_command_samples(struct iax_session *i, char type, int command, unsigned int ts, unsigned char *data, int datalen, int samples)
{
	return iax_send(i, type, command, data, datalen, ts,
			-1, /* seqno */
			0,  /* immediate */
			0,  /* transfer */
			0,  /* final */
			0,  /* fullframe */
			samples);
}


int iax_transfer(struct iax_session *session, const char *number)
{
	static int res;				//Return Code
	struct iax_ie_data ied;			//IE Data Structure (Stuff To Send)

	// Clear The Memory Used For IE Buffer
	memset(&ied, 0, sizeof(ied));

	// Copy The Transfer Destination Into The IE Structure
	iax_ie_append_str(&ied, IAX_IE_CALLED_NUMBER, number);

	// Send The Transfer Command - Asterisk Will Handle The Rest!
	res = send_command(session, AST_FRAME_IAX, IAX_COMMAND_TRANSFER, 0, ied.buf, ied.pos);

	// Return Success
	return 0;
}

static void stop_transfer(struct iax_session *session)
{
	struct iax_sched *sch;

	sch = schedq;
	while(sch) {
		if (sch->frame && sch->frame->session == session)
			sch->frame->retries = -1;
		sch = sch->next;
	}
}

static void complete_transfer(struct iax_session *session, int peercallno, int xfr2peer, int preserveSeq)
{
	jb_frame frame;

	session->peercallno = peercallno;
	/* Change from transfer to session now */
	if (xfr2peer) {
		memcpy(&session->peeraddr, &session->transfer, sizeof(session->peeraddr));
		memset(&session->transfer, 0, sizeof(session->transfer));
		session->transferring = TRANSFER_NONE;
		session->transferpeer = 0;
		session->transfer_moh = 0;
		/* Force retransmission of a real voice packet, and reset all timing */
		session->svoiceformat = -1;
		session->voiceformat = 0;
		session->svideoformat = -1;
		session->videoformat = 0;
	}

	memset(&session->rxcore, 0, sizeof(session->rxcore));
	memset(&session->offset, 0, sizeof(session->offset));

	/* Reset jitterbuffer */
	while(jb_getall(session->jb,&frame) == JB_OK)
		iax_event_free((struct iax_event *)frame.data);

	jb_reset(session->jb);

	if (! preserveSeq)
	{
		/* Reset sequence numbers */
		session->aseqno = 0;
		session->oseqno = 0;
		session->iseqno = 0;
	}

	session->lastsent = 0;
	session->last_ts = 0;
	session->pingtime = 30;
	/* We have to dump anything we were going to (re)transmit now that we've been
	   transferred since they're all invalid and for the old host. */
	stop_transfer(session);
}	/* complete_transfer */

int iax_setup_transfer(struct iax_session *org_session, struct iax_session *new_session)
{
	int res;
	struct iax_ie_data ied0;
	struct iax_ie_data ied1;

	struct iax_session *s0 = org_session;
	struct iax_session *s1 = new_session;

	int transfer_id = 1 + (int)(32767.0 * (iax_random() / (RAND_MAX + 1.0)));

	memset(&ied0, 0, sizeof(ied0));
	memset(&ied1, 0, sizeof(ied1));

	/* reversed setup */
	iax_ie_append_addr(&ied0, IAX_IE_APPARENT_ADDR, &s1->peeraddr);
	iax_ie_append_short(&ied0, IAX_IE_CALLNO, s1->peercallno);
	iax_ie_append_int(&ied0, IAX_IE_TRANSFERID, transfer_id);

	iax_ie_append_addr(&ied1, IAX_IE_APPARENT_ADDR, &s0->peeraddr);
	iax_ie_append_short(&ied1, IAX_IE_CALLNO, s0->peercallno);
	iax_ie_append_int(&ied1, IAX_IE_TRANSFERID, transfer_id);

	s0->transfer = s1->peeraddr;
	s1->transfer = s0->peeraddr;

	s0->transferid = transfer_id;
	s1->transferid = transfer_id;

	s0->transfercallno = s0->peercallno;
	s1->transfercallno = s1->peercallno;

	s0->transferring = TRANSFER_BEGIN;
	s1->transferring = TRANSFER_BEGIN;

	s0->transferpeer = s1->callno;
	s1->transferpeer = s0->callno;
#ifdef DEBUG_SUPPORT
	if (debug) {
		DEBU(G "iax_setup_transfer(%d, %d) transfer_id=%d\n", s0->callno, s1->callno, transfer_id);
		DEBU(G "\torg: callno=%d peercallno=%d peeraddr=%s peerport=%d\n", s0->callno, s0->peercallno, inet_ntoa(s0->peeraddr.sin_addr), ntohs(s0->peeraddr.sin_port));
		DEBU(G "\tnew: callno=%d peercallno=%d peeraddr=%s peerport=%d\n", s1->callno, s1->peercallno, inet_ntoa(s1->peeraddr.sin_addr), ntohs(s1->peeraddr.sin_port));
	}
#endif

	res = send_command(s0, AST_FRAME_IAX, IAX_COMMAND_TXREQ, 0, ied0.buf, ied0.pos);
	if (res < 0) {
		return -1;
	}

	res = send_command(s1, AST_FRAME_IAX, IAX_COMMAND_TXREQ, 0, ied1.buf, ied1.pos);
	if (res < 0) {
		return -1;
	}

	return 0;
}

static int iax_finish_transfer(struct iax_session *s, short new_peer)
{
	int res;
	struct iax_ie_data ied;

	memset(&ied, 0, sizeof(ied));

	iax_ie_append_short(&ied, IAX_IE_CALLNO, new_peer);

	res = send_command(s, AST_FRAME_IAX, IAX_COMMAND_TXREL, 0, ied.buf, ied.pos);

	complete_transfer(s, new_peer, 0, 1);

	return res;

}

static struct iax_session *iax_find_session2(short callno)
{
	struct iax_session *cur = sessions;

	while(cur) {
		if (callno == cur->callno && callno != 0)  {
			return cur;
		}
		cur = cur->next;
	}

	return NULL;
}

static int iax_handle_txready(struct iax_session *s)
{
	struct iax_session *s0, *s1;
	short s0_org_peer, s1_org_peer;

	if (s->transfer_moh) {
		s->transfer_moh = 0;
		iax_unquelch(s);
	}

	complete_transfer(s, s->peercallno, 0, 1);

	s->transferring = TRANSFER_REL;

	s0 = s;
	s1 = iax_find_session2(s0->transferpeer);

	if (s1 != NULL &&
	    s1->callno == s0->transferpeer &&
	    s0->transferring == TRANSFER_REL &&
	    s1->transferring == TRANSFER_REL) {

		s0_org_peer = s0->peercallno;
		s1_org_peer = s1->peercallno;

		iax_finish_transfer(s0, s1_org_peer);
		iax_finish_transfer(s1, s0_org_peer);
		return 1;
	}

	return 0;
}

static void iax_handle_txreject(struct iax_session *s)
{
	struct iax_session *s0, *s1;

	s0 = s;
	s1 = iax_find_session2(s0->transferpeer);
	if (s1 != NULL &&
		 s0->transferpeer == s1->callno &&
		 s1->transferring) {
		if (s1->transfer_moh) {
			s1->transfer_moh = 0;
			send_command_immediate(s1, AST_FRAME_IAX, IAX_COMMAND_UNQUELCH, 0, NULL, 0, s1->iseqno);
		}
	}
	if (s0->transfer_moh) {
		s0->transfer_moh = 0;
		send_command_immediate(s0, AST_FRAME_IAX, IAX_COMMAND_UNQUELCH, 0, NULL, 0, s0->iseqno);
	}

	memset(&s->transfer, 0, sizeof(s->transfer));
	s->transferring = TRANSFER_NONE;
	s->transferpeer = 0;
	s->transfer_moh = 0;
}

static void destroy_session(struct iax_session *session)
{
	struct iax_session *cur, *prev=NULL;
	struct iax_sched *curs, *prevs=NULL, *nexts=NULL;
	curs = schedq;
	while(curs) {
		nexts = curs->next;
		if (curs->frame && curs->frame->session == session) {
			/* Just mark these frames as if they've been sent */
			curs->frame->retries = -1;
		} else if (curs->event && curs->event->session == session) {
			if (prevs)
				prevs->next = nexts;
			else
				schedq = nexts;
			if (curs->event)
				iax_event_free(curs->event);
			free(curs);
		} else {
			prevs = curs;
		}
		curs = nexts;
	}

	cur = sessions;
	while(cur) {
		if (cur == session) {
			jb_frame frame;

			if (prev)
				prev->next = session->next;
			else
				sessions = session->next;

			while(jb_getall(session->jb,&frame) == JB_OK)
				iax_event_free((struct iax_event *)frame.data);

			jb_destroy(session->jb);

			free(session);
			return;
		}
		prev = cur;
		cur = cur->next;
	}
}

static int iax_send_lagrp(struct iax_session *session, unsigned int ts);
static int iax_send_pong(struct iax_session *session, unsigned int ts);

static struct iax_event *handle_event(struct iax_event *event)
{
	/* We have a candidate event to be delievered.  Be sure
	   the session still exists. */
	if (event)
	{
		if ( event->etype == IAX_EVENT_NULL ) return event;
		if (iax_session_valid(event->session))
		{
			/* Lag requests are never actually sent to the client, but
			   other than that are handled as normal packets */
			switch(event->etype)
			{
				/* the user on the outside may need to look at the session so we will not free
				   it here anymore we will test for hangup event in iax_event_free and do it
				   there.
				 */
			case IAX_EVENT_REJECT:
			case IAX_EVENT_HANGUP:
				/* Destroy this session -- it's no longer valid */
				destroy_session(event->session);
				return event;
			case IAX_EVENT_LAGRQ:
				event->etype = IAX_EVENT_LAGRP;
				iax_send_lagrp(event->session, event->ts);
				iax_event_free(event);
				break;
			case IAX_EVENT_PING:
				event->etype = IAX_EVENT_PONG;
				iax_send_pong(event->session, event->ts);
				iax_event_free(event);
				break;
			case IAX_EVENT_POKE:
				event->etype = IAX_EVENT_PONG;
				iax_send_pong(event->session, event->ts);
				destroy_session(event->session);
				iax_event_free(event);
				break;
			default:
				return event;
			}
		} else
			iax_event_free(event);
	}
	return NULL;
}

static int iax2_vnak(struct iax_session *session)
{
	/* send vnak just once for a given sequence number */
	if ( (unsigned char)(session->lastvnak - session->iseqno) < 128 )
	{
		return 0;
	}

	session->lastvnak = session->iseqno;
	return send_command_immediate(session, AST_FRAME_IAX, IAX_COMMAND_VNAK, 0, NULL, 0, session->iseqno);
}

int iax_send_dtmf(struct iax_session *session, char digit)
{
	return send_command(session, AST_FRAME_DTMF, digit, 0, NULL, 0);
}

int iax_send_voice(struct iax_session *session, int format, unsigned char *data, int datalen, int samples)
{
	/* Send a (possibly compressed) voice frame */
	if (!session->quelch)
		return send_command_samples(session, AST_FRAME_VOICE, format, 0, data, datalen, samples);
	return 0;
}

int iax_send_cng(struct iax_session *session, int level, unsigned char *data,
		int datalen)
{
#ifdef USE_VOICE_TS_PREDICTION
	session->notsilenttx = 0;
#endif
	return send_command(session, AST_FRAME_CNG, level, 0, data, datalen);
}

int iax_send_image(struct iax_session *session, int format, unsigned char *data,
		int datalen)
{
	/* Send an image frame */
	return send_command(session, AST_FRAME_IMAGE, format, 0, data, datalen);
}

int iax_send_video(struct iax_session *session, int format, unsigned char *data,
		int datalen, int fullframe)
{
	if (!session->quelch)
	{
		int res = send_command_video(session, AST_FRAME_VIDEO, format,
				0, data, datalen, fullframe);
		return res;
	}
	return 0;
}

int iax_send_video_trunk(struct iax_session *session, int format,
		char *data, int datalen, int fullframe, int ntrunk)
{
	static int my_lastts = 0;

	if ( ntrunk == 0 )
		my_lastts = calc_timestamp(session, 0, 0);

	if ( !session->quelch )
	{
		return send_command_video(session, AST_FRAME_VIDEO, format,
				my_lastts, (unsigned char *)data, datalen,
				fullframe);
	}
	return 0;
}

int iax_video_bypass_jitter(struct iax_session *s, int mode)
{
	video_bypass_jitterbuffer = mode;
	return 0;
}

int iax_register(struct iax_session *session, const char *server, const char *peer, const char *secret, int refresh)
{
	/* Send a registration request */
	char tmp[256];
	char *p;
	int res;
	int portno = IAX_DEFAULT_PORTNO;
	struct iax_ie_data ied;
	struct hostent *hp;

	tmp[255] = '\0';
	strncpy(tmp, server, sizeof(tmp) - 1);
	p = strchr(tmp, ':');
	if (p) {
		*p = '\0';
		portno = atoi(p+1);
	}

	memset(&ied, 0, sizeof(ied));
	if (secret)
		strncpy(session->secret, secret, sizeof(session->secret) - 1);
	else
		strcpy(session->secret, "");

	memset(&session->unregreason, 0, sizeof(session->unregreason));

	/* Connect first */
	hp = gethostbyname(tmp);
	if (!hp) {
		snprintf(iax_errstr, sizeof(iax_errstr), "Invalid hostname: %s", tmp);
		return -1;
	}
	memcpy(&session->peeraddr.sin_addr, hp->h_addr, sizeof(session->peeraddr.sin_addr));
	session->peeraddr.sin_port = htons(portno);
	session->peeraddr.sin_family = AF_INET;
	strncpy(session->username, peer, sizeof(session->username) - 1);
	session->refresh = refresh;
	iax_ie_append_str(&ied, IAX_IE_USERNAME, peer);
	iax_ie_append_short(&ied, IAX_IE_REFRESH, refresh);
	res = send_command(session, AST_FRAME_IAX, IAX_COMMAND_REGREQ, 0, ied.buf, ied.pos);
	return res;
}

int iax_unregister(struct iax_session *session, const char *server, const char *peer, const char *secret, const char *reason)
{
	/* Send an unregistration request */
	char tmp[256];
	char *p;
	int portno = IAX_DEFAULT_PORTNO;
	struct iax_ie_data ied;
	struct hostent *hp;

	tmp[255] = '\0';
	strncpy(tmp, server, sizeof(tmp) - 1);
	p = strchr(tmp, ':');
	if (p) {
		*p = '\0';
		portno = atoi(p+1);
	}

	memset(&ied, 0, sizeof(ied));
	if (secret)
		strncpy(session->secret, secret, sizeof(session->secret) - 1);
	else
		strcpy(session->secret, "");

	if (reason && strlen(reason))
		strncpy(session->unregreason, reason, sizeof(session->unregreason) - 1);
	else
		strcpy(session->unregreason, "Unspecified");

	/* Connect first */
	hp = gethostbyname(tmp);
	if (!hp) {
		snprintf(iax_errstr, sizeof(iax_errstr), "Invalid hostname: %s", tmp);
		return -1;
	}
	memcpy(&session->peeraddr.sin_addr, hp->h_addr, sizeof(session->peeraddr.sin_addr));
	session->peeraddr.sin_port = htons(portno);
	session->peeraddr.sin_family = AF_INET;
	strncpy(session->username, peer, sizeof(session->username) - 1);
	iax_ie_append_str(&ied, IAX_IE_USERNAME, peer);
	iax_ie_append_str(&ied, IAX_IE_CAUSE, session->unregreason);
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_REGREL, 0, ied.buf, ied.pos);
}

int iax_reject(struct iax_session *session, char *reason)
{
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_str(&ied, IAX_IE_CAUSE, reason ? reason : "Unspecified");
	return send_command_final(session, AST_FRAME_IAX, IAX_COMMAND_REJECT, 0, ied.buf, ied.pos);
}

int iax_hangup(struct iax_session *session, char *byemsg)
{
	struct iax_ie_data ied;
	iax_sched_del(NULL, NULL, send_ping, (void *) session, 1);
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_str(&ied, IAX_IE_CAUSE, byemsg ? byemsg : "Normal clearing");
	return send_command_final(session, AST_FRAME_IAX, IAX_COMMAND_HANGUP, 0, ied.buf, ied.pos);
}

int iax_sendurl(struct iax_session *session, char *url)
{
	return send_command(session, AST_FRAME_HTML, AST_HTML_URL, 0,
			(unsigned char *)url, (int)strlen(url));
}

int iax_ring_announce(struct iax_session *session)
{
	return send_command(session, AST_FRAME_CONTROL, AST_CONTROL_RINGING, 0, NULL, 0);
}

int iax_lag_request(struct iax_session *session)
{
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_LAGRQ, 0, NULL, 0);
}

int iax_busy(struct iax_session *session)
{
	return send_command(session, AST_FRAME_CONTROL, AST_CONTROL_BUSY, 0, NULL, 0);
}

int iax_congestion(struct iax_session *session)
{
	return send_command(session, AST_FRAME_CONTROL, AST_CONTROL_CONGESTION, 0, NULL, 0);
}


int iax_accept(struct iax_session *session, int format)
{
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_int(&ied, IAX_IE_FORMAT, format);
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_ACCEPT, 0, ied.buf, ied.pos);
}

int iax_answer(struct iax_session *session)
{
	return send_command(session, AST_FRAME_CONTROL, AST_CONTROL_ANSWER, 0, NULL, 0);
}

int iax_key_radio(struct iax_session *session)
{
	return send_command(session, AST_FRAME_CONTROL, AST_CONTROL_KEY, 0, NULL, 0);
}

int iax_unkey_radio(struct iax_session *session)
{
	return send_command(session, AST_FRAME_CONTROL, AST_CONTROL_UNKEY, 0, NULL, 0);
}

int iax_load_complete(struct iax_session *session)
{
	return send_command(session, AST_FRAME_HTML, AST_HTML_LDCOMPLETE, 0, NULL, 0);
}

int iax_send_url(struct iax_session *session, const char *url, int link)
{
	return send_command(session, AST_FRAME_HTML,
			link ? AST_HTML_LINKURL : AST_HTML_URL, 0,
			(unsigned char *)url, (int)strlen(url));
}

int iax_send_text(struct iax_session *session, const char *text)
{
	return send_command(session, AST_FRAME_TEXT, 0, 0,
			(unsigned char *)text, (int)strlen(text) + 1);
}

int iax_send_unlink(struct iax_session *session)
{
	return send_command(session, AST_FRAME_HTML, AST_HTML_UNLINK, 0, NULL, 0);
}

int iax_send_link_reject(struct iax_session *session)
{
	return send_command(session, AST_FRAME_HTML, AST_HTML_LINKREJECT, 0, NULL, 0);
}

static int iax_send_pong(struct iax_session *session, unsigned int ts)
{
        struct iax_ie_data ied;
	jb_info stats;

        memset(&ied, 0, sizeof(ied));

	jb_getinfo(session->jb, &stats);

	iax_ie_append_int(&ied,IAX_IE_RR_JITTER, stats.jitter);
	/* XXX: should be short-term loss pct.. */
	if(stats.frames_in == 0) stats.frames_in = 1;
	iax_ie_append_int(&ied,IAX_IE_RR_LOSS,
			((0xff & (stats.losspct/1000)) << 24 |
			 (stats.frames_lost & 0x00ffffff)));
	iax_ie_append_int(&ied,IAX_IE_RR_PKTS, stats.frames_in);
	iax_ie_append_short(&ied,IAX_IE_RR_DELAY,
			(unsigned short)(stats.current - stats.min));
	iax_ie_append_int(&ied,IAX_IE_RR_DROPPED, stats.frames_dropped);
	iax_ie_append_int(&ied,IAX_IE_RR_OOO, stats.frames_ooo);

	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_PONG, ts, ied.buf, ied.pos);
}

/* external API; deprecated since we send pings ourselves now (finally) */
int iax_send_ping(struct iax_session *session)
{
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_PING, 0, NULL, 0);
}

/* scheduled ping sender; sends ping, then reschedules */
static void send_ping(void *s)
{
	struct iax_session *session = (struct iax_session *)s;

	/* important, eh? */
	if(!iax_session_valid(session)) return;

	send_command(session, AST_FRAME_IAX, IAX_COMMAND_PING, 0, NULL, 0);
	session->pingid = iax_sched_add(NULL,NULL, send_ping, (void *)session, ping_time * 1000);
	return;
}

static int iax_send_lagrp(struct iax_session *session, unsigned int ts)
{
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_LAGRP, ts, NULL, 0);
}

static int iax_send_txcnt(struct iax_session *session)
{
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_int(&ied, IAX_IE_TRANSFERID, session->transferid);
	return send_command_transfer(session, AST_FRAME_IAX, IAX_COMMAND_TXCNT, ied.buf, ied.pos);
}

static int iax_send_txrej(struct iax_session *session)
{
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_int(&ied, IAX_IE_TRANSFERID, session->transferid);
	return send_command_transfer(session, AST_FRAME_IAX, IAX_COMMAND_TXREJ, ied.buf, ied.pos);
}

static int iax_send_txaccept(struct iax_session *session)
{
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_int(&ied, IAX_IE_TRANSFERID, session->transferid);
	return send_command_transfer(session, AST_FRAME_IAX, IAX_COMMAND_TXACC, ied.buf, ied.pos);
}

static int iax_send_txready(struct iax_session *session)
{
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	/* see asterisk chan_iax2.c */
	iax_ie_append_short(&ied, IAX_IE_CALLNO, session->callno);
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_TXREADY, 0, ied.buf, ied.pos);
}

int iax_auth_reply(struct iax_session *session, char *password, char *challenge, int methods)
{
	char reply[16];
	struct MD5Context md5;
	char realreply[256];
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	if ((methods & IAX_AUTH_MD5) && challenge) {
		MD5Init(&md5);
		MD5Update(&md5, (const unsigned char *) challenge,
				(unsigned int)strlen(challenge));
		MD5Update(&md5, (const unsigned char *) password,
				(unsigned int)strlen(password));
		MD5Final((unsigned char *) reply, &md5);
		memset(realreply, 0, sizeof(realreply));
		convert_reply(realreply, (unsigned char *) reply);
		iax_ie_append_str(&ied, IAX_IE_MD5_RESULT, realreply);
	} else {
		iax_ie_append_str(&ied, IAX_IE_PASSWORD, password);
	}
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_AUTHREP, 0, ied.buf, ied.pos);
}

static int iax_regauth_reply(struct iax_session *session, char *password, char *challenge, int methods)
{
	char reply[16];
	struct MD5Context md5;
	char realreply[256];
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_str(&ied, IAX_IE_USERNAME, session->username);
	if ((methods & IAX_AUTHMETHOD_MD5) && challenge) {
		MD5Init(&md5);
		MD5Update(&md5, (const unsigned char *) challenge,
				(unsigned int)strlen(challenge));
		MD5Update(&md5, (const unsigned char *) password,
				(unsigned int)strlen(password));
		MD5Final((unsigned char *) reply, &md5);
		memset(realreply, 0, sizeof(realreply));
		convert_reply(realreply, (unsigned char *) reply);
		iax_ie_append_str(&ied, IAX_IE_MD5_RESULT, realreply);
	} else {
		iax_ie_append_str(&ied, IAX_IE_PASSWORD, password);
	}
	if (strlen(session->unregreason)) {		/* Non-zero unregreason length indicates REGREL */
		iax_ie_append_str(&ied, IAX_IE_CAUSE, session->unregreason);
		return send_command(session, AST_FRAME_IAX, IAX_COMMAND_REGREL, 0, ied.buf, ied.pos);
	} else {
		iax_ie_append_short(&ied, IAX_IE_REFRESH, session->refresh);
		return send_command(session, AST_FRAME_IAX, IAX_COMMAND_REGREQ, 0, ied.buf, ied.pos);
	}
}


int iax_dial(struct iax_session *session, char *number)
{
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_str(&ied, IAX_IE_CALLED_NUMBER, number);
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_DIAL, 0, ied.buf, ied.pos);
}

int iax_quelch(struct iax_session *session)
{
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_QUELCH, 0, NULL, 0);
}

int iax_unquelch(struct iax_session *session)
{
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_UNQUELCH, 0, NULL, 0);
}

int iax_dialplan_request(struct iax_session *session, char *number)
{
	struct iax_ie_data ied;
	memset(&ied, 0, sizeof(ied));
	iax_ie_append_str(&ied, IAX_IE_CALLED_NUMBER, number);
	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_DPREQ, 0, ied.buf, ied.pos);
}

static inline int which_bit(unsigned int i)
{
	char x;
	for(x = 0; x < 32; x++) {
		if ((1U << x) == i) {
			return x + 1;
		}
	}
	return 0;
}

char iax_pref_codec_add(struct iax_session *session, unsigned int format)
{
	int diff = (int) 'A';
	session->codec_order[session->codec_order_len++] = (which_bit(format)) + diff;
	session->codec_order[session->codec_order_len] = '\0';
	return session->codec_order[session->codec_order_len-1];
}


void iax_pref_codec_del(struct iax_session *session, unsigned int format)
{
	int diff = (int) 'A';
	int x;
	char old[32];
	char remove = which_bit(format) + diff;

	strncpy(old, session->codec_order, sizeof(old));
	session->codec_order_len = 0;

	for (x = 0;  x < (int) strlen(old);  x++) {
		if (old[x] != remove) {
			session->codec_order[session->codec_order_len++] = old[x];
		}
	}
	session->codec_order[session->codec_order_len] = '\0';
}

int iax_pref_codec_get(struct iax_session *session, unsigned int *array, int len)
{
	int diff = (int) 'A';
	int x;

	for (x = 0; x < session->codec_order_len && x < len; x++) {
		array[x] = (1 << (session->codec_order[x] - diff - 1));
	}

	return x;
}

int iax_call(struct iax_session *session, const char *cidnum, const char *cidname, const char *ich, const char *lang, int wait, int formats, int capabilities)
{
	char tmp[256]="";
	char *part1, *part2;
	int res;
	int portno;
	char *username, *hostname, *secret, *context, *exten, *dnid;
	struct iax_ie_data ied;
	struct hostent *hp;
	/* We start by parsing up the temporary variable which is of the form of:
	   [user@]peer[:portno][/exten[@context]] */
	if (!ich) {
		IAXERROR "Invalid IAX Call Handle\n");
		DEBU(G "Invalid IAX Call Handle\n");
		return -1;
	}
	memset(&ied, 0, sizeof(ied));
	strncpy(tmp, ich, sizeof(tmp) - 1);
	iax_ie_append_short(&ied, IAX_IE_VERSION, IAX_PROTO_VERSION);
	if (cidnum)
		iax_ie_append_str(&ied, IAX_IE_CALLING_NUMBER, cidnum);
	if (cidname)
		iax_ie_append_str(&ied, IAX_IE_CALLING_NAME, cidname);

	if (session->codec_order_len) {
		iax_ie_append_str(&ied, IAX_IE_CODEC_PREFS, session->codec_order);
	}

	session->capability = capabilities;
	session->pingid = iax_sched_add(NULL,NULL, send_ping, (void *)session, 2 * 1000);

	/* XXX We should have a preferred format XXX */
	iax_ie_append_int(&ied, IAX_IE_FORMAT, formats);
	iax_ie_append_int(&ied, IAX_IE_CAPABILITY, capabilities);
	if (lang)
		iax_ie_append_str(&ied, IAX_IE_LANGUAGE, lang);

	/* Part 1 is [user[:password]@]peer[:port] */
	part1 = strtok(tmp, "/");

	/* Part 2 is exten[@context] if it is anything all */
	part2 = strtok(NULL, "/");

	if (strchr(part1, '@')) {
		username = strtok(part1, "@");
		hostname = strtok(NULL, "@");
	} else {
		username = NULL;
		hostname = part1;
	}

	if (username && strchr(username, ':')) {
		username = strtok(username, ":");
		secret = strtok(NULL, ":");
	} else
		secret = NULL;

	if(username)
		strncpy(session->username, username, sizeof(session->username) - 1);

	if(secret)
		strncpy(session->secret, secret, sizeof(session->secret) - 1);

	if (strchr(hostname, ':')) {
		strtok(hostname, ":");
		portno = atoi(strtok(NULL, ":"));
	} else {
		portno = IAX_DEFAULT_PORTNO;
	}
	if (part2) {
		exten = strtok(part2, "@");
		dnid = exten;
		context = strtok(NULL, "@");
	} else {
		exten = NULL;
		dnid = NULL;
		context = NULL;
	}
	if (username)
		iax_ie_append_str(&ied, IAX_IE_USERNAME, username);
	if (exten && strlen(exten))
		iax_ie_append_str(&ied, IAX_IE_CALLED_NUMBER, exten);
	if (dnid && strlen(dnid))
		iax_ie_append_str(&ied, IAX_IE_DNID, dnid);
	if (context && strlen(context))
		iax_ie_append_str(&ied, IAX_IE_CALLED_CONTEXT, context);

	/* Setup host connection */
	hp = gethostbyname(hostname);
	if (!hp) {
		snprintf(iax_errstr, sizeof(iax_errstr), "Invalid hostname: %s", hostname);
		return -1;
	}
	memcpy(&session->peeraddr.sin_addr, hp->h_addr, sizeof(session->peeraddr.sin_addr));
	session->peeraddr.sin_port = htons(portno);
	session->peeraddr.sin_family = AF_INET;
	res = send_command(session, AST_FRAME_IAX, IAX_COMMAND_NEW, 0, ied.buf, ied.pos);
	if (res < 0)
		return res;
	if (wait) {
		DEBU(G "Waiting not yet implemented\n");
		return -1;
	}
	return res;
}

static int calc_rxstamp(struct iax_session *session)
{
	struct timeval now = iax_tvnow();

	if (iax_tvzero(session->rxcore))
		session->rxcore = now;

	return iax_tvdiff_ms(now, session->rxcore);
}

/* splitted match into 2 passes otherwise causing problem of matching
   up the wrong session using the dcallno and the peercallno because
   during a transfer (2 IAX channels on the same client/system) the
   same peercallno (from two different asterisks) exist in more than
   one session.
 */
static int forward_match(struct sockaddr_in *sin, short callno, short dcallno, struct iax_session *cur)
{
	if (inaddrcmp(&cur->transfer, sin) == 0 && cur->transferring) {
		/* We're transferring */
		if (dcallno == cur->callno)
			return 1;
	}

	if (inaddrcmp(&cur->peeraddr, sin) == 0) {
		if (dcallno == cur->callno && dcallno != 0)  {
			/* That's us.  Be sure we keep track of the peer call number */
			if (cur->peercallno == 0) {
				cur->peercallno = callno;
			}
			else if ( cur->peercallno != callno ) 
			{
				// print a warning when the callno's don't match
				fprintf( stderr, "WARNING: peercallno does not match callno"
					", peercallno => %d, callno => %d, dcallno => %d\n",
					cur->peercallno, callno, dcallno ) ;
				return 0 ;
			}
			return 1;
		}
	}

	return 0;
}

static int reverse_match(struct sockaddr_in *sin, short callno, struct iax_session *cur)
{
	if (inaddrcmp(&cur->transfer, sin) == 0 && cur->transferring) {
		/* We're transferring */
		if (callno == cur->peercallno)
			return 1;
	}

	if (inaddrcmp(&cur->peeraddr, sin) == 0) {
		if (callno == cur->peercallno)
			return 1;
	}

	return 0;
}

static struct iax_session *iax_find_session(struct sockaddr_in *sin,
		short callno,
		short dcallno,
		int makenew)
{
	struct iax_session *cur = sessions;
	while (cur) {
		if (forward_match(sin, callno, dcallno, cur))
			return cur;
		cur = cur->next;
	}

	cur = sessions;
	while (cur) {
		if (reverse_match(sin, callno, cur))
			return cur;
		cur = cur->next;
	}

	if (makenew && !dcallno) {
		cur = iax_session_new();
		cur->peercallno = callno;
		cur->peeraddr.sin_addr.s_addr = sin->sin_addr.s_addr;
		cur->peeraddr.sin_port = sin->sin_port;
		cur->peeraddr.sin_family = AF_INET;
		cur->pingid = iax_sched_add(NULL,NULL, send_ping, (void *)cur, 2 * 1000);
		DEBU(G "Making new session, peer callno %d, our callno %d\n", callno, cur->callno);
	} else {
		DEBU(G "No session, peer = %d, us = %d\n", callno, dcallno);
	}
	return cur;
}

static long unwrap_timestamp(long ts, long last, int is_video)
{
	const int ts_shift = is_video ? 15 : 16;
	const long lower_mask = (1 << ts_shift) - 1;
	const long upper_mask = ~lower_mask;

	if ( (ts & upper_mask) == (last & upper_mask) ) {
		const long x = ts - last;
		const long threshold = is_video ? 25000 : 50000;

		if (x < -threshold) {
			/* Sudden big jump backwards in timestamp: What likely
			 * happened here is that miniframe timestamp has
			 * circled but we haven't gotten the update from the
			 * main packet. We'll just pretend that we did, and
			 * update the timestamp appropriately.
			 */
			ts = ((last & upper_mask) + (1 << ts_shift)) | (ts & lower_mask);
			DEBU(G "schedule_delivery: pushed forward timestamp\n");
		} else if (x > threshold) {
			/* Sudden apparent big jump forwards in timestamp:
			 * What's likely happened is this is an old miniframe
			 * belonging to the previous top 15-bit or 16-bit
			 * timestamp that has turned up out of order. Adjust
			 * the timestamp appropriately.
			 */
			ts = ((last & upper_mask) - (1 << ts_shift)) | (ts & lower_mask);
			DEBU(G "schedule_delivery: pushed back timestamp\n");
		}
	}
	return ts;
}


static struct iax_event *schedule_delivery(struct iax_event *e, unsigned int ts, int updatehistory)
{
	/*
	 * This is the core of the IAX jitterbuffer delivery mechanism:
	 * Dynamically adjust the jitterbuffer and decide how long to wait
	 * before delivering the packet.
	 */

	int type = JB_TYPE_CONTROL;
	int ms = 0;

	/* insert into jitterbuffer */
	/* TODO: Perhaps we could act immediately if it's not droppable and late */
	if ( !iax_use_jitterbuffer ||
			(e->etype == IAX_EVENT_VIDEO &&
			 video_bypass_jitterbuffer) )
	{
		iax_sched_add(e, NULL, NULL, NULL, 0);
		return NULL;
	}

	if (e->etype == IAX_EVENT_VOICE)
	{
		const int samples = get_sample_cnt(e);

		if ( samples < 0 )
		{
			/* If get_sample_cnt fails, then something very bad
			 * is occurring, drop the frame and log the problem.
			 */
			fprintf(stderr, "iax2: get_sample_cnt failed %d\n",
					samples);
			iax_event_free(e);
			return NULL;
		}

		/* The frame time only has an effect for voice */
		ms = samples * 1000 / 8000;

		type = JB_TYPE_VOICE;
	}
	else if (e->etype == IAX_EVENT_VIDEO)
	{
		type = JB_TYPE_VIDEO;
	}
	else if (e->etype == IAX_EVENT_CNG)
	{
		type = JB_TYPE_SILENCE;
	}

	/* unwrap timestamp */
	ts = unwrap_timestamp(ts, e->session->last_ts,
			e->etype == IAX_EVENT_VIDEO);

	/* move forward last_ts if it's greater. We do this _after_
	 * unwrapping, because asterisk _still_ has cases where it
	 * doesn't send full frames when it ought to */
	if ( ts > e->session->last_ts )
		e->session->last_ts = ts;

	if ( jb_put(e->session->jb, e, type, ms, ts,
				calc_rxstamp(e->session)) == JB_DROP )
	{
		iax_event_free(e);
	}

	return NULL;
}

static int uncompress_subclass(unsigned char csub)
{
	/* If the SC_LOG flag is set, return 2^csub otherwise csub */
	if (csub & IAX_FLAG_SC_LOG)
		return 1 << (csub & ~IAX_FLAG_SC_LOG & IAX_MAX_SHIFT);
	else
		return csub;
}

static void iax_handle_vnak(struct iax_session *session, struct ast_iax2_full_hdr *fh)
{
	struct iax_sched *sch = schedq;
	struct iax_sched *retrans_list = NULL;
	const unsigned char vnak_seqno = fh->iseqno - session->rseqno;

	/* According to the IAX2 02 draft, we MUST immediately retransmit all
	 * frames with higher sequence number than the VNAK's iseqno. However,
	 * it seems that the right thing to do would be to retransmit frames
	 * with sequence numbers higher OR EQUAL to VNAK's iseqno.
	 */
	while ( sch != NULL )
	{
		if ( sch->frame != NULL &&
		     sch->frame->session == session
		   )
		{
			/* We want to check if our frame's oseqno is greater or
			 * equal than the VNAK's iseqno, but we need to take
			 * into account sequence number wrap-arounds.
			 * session->rseqno is our last acknowledged sequence
			 * number, so we use that as a base.
			 */
			const unsigned char frame_seqno =
				sch->frame->oseqno - session->rseqno;

			if ( vnak_seqno <= frame_seqno )
			{
				/* We cannot retransmit immediately since the
				 * frames are ordered by retransmit time. We
				 * need to first collect them and arrange them
				 * in ascending order by their oseqno. */
				struct iax_sched *tmp =
					calloc(1, sizeof(struct iax_sched));
				tmp->frame = sch->frame;

				if ( retrans_list == NULL ||
				     (retrans_list->frame->oseqno - session->rseqno) > frame_seqno
				   )
				{
					tmp->next = retrans_list;
					retrans_list = tmp;
				}
				else
				{
					struct iax_sched *item = retrans_list;
					while ( item != NULL )
					{
						if ( item->next == NULL ||
						     item->next->frame->oseqno - session->rseqno > frame_seqno
						   )
						{
							tmp->next = item->next;
							item->next = tmp;
							break;
						}
						item = item->next;
					}
				}
			}
		}
		sch = sch->next;
	}

	/* Transmit collected frames and free the space */
	while ( retrans_list != NULL )
	{
		struct iax_sched * tmp = retrans_list;
		iax_xmit_frame(retrans_list->frame);
		retrans_list = retrans_list->next;
		free(tmp);
	}
}

static struct iax_event *iax_header_to_event(struct iax_session *session, struct ast_iax2_full_hdr *fh, int datalen, struct sockaddr_in *sin)
{
	struct iax_event *e;
	struct iax_sched *sch;
	unsigned int ts;
	int subclass;
	int nowts;
	int updatehistory = 1;
	ts = ntohl(fh->ts);

	if (fh->type==AST_FRAME_VIDEO)
		subclass = uncompress_subclass(fh->csub & ~0x40) | ((fh->csub >> 6) & 0x1);
	else
		subclass = uncompress_subclass(fh->csub);

	/* don't run last_ts backwards; i.e. for retransmits and the like */
	if (ts > session->last_ts &&
	    (fh->type == AST_FRAME_IAX &&
	     subclass != IAX_COMMAND_ACK &&
	     subclass != IAX_COMMAND_PONG &&
	     subclass != IAX_COMMAND_LAGRP))
	{
		session->last_ts = ts;
	}

#ifdef DEBUG_SUPPORT
	if (debug)
		iax_showframe(NULL, fh, 1, sin, datalen);
#endif

	/* Get things going with it, timestamp wise, if we haven't already. */

	/* Handle implicit ACKing unless this is an INVAL, and only if this is
	 * from the real peer, not the transfer peer */
	if ( !inaddrcmp(sin, &session->peeraddr) &&
	     ( subclass != IAX_COMMAND_INVAL ||
	       fh->type != AST_FRAME_IAX
	     )
	   )
	{
		unsigned char x;
		/* XXX This code is not very efficient.  Surely there is a better way which still
			properly handles boundary conditions? XXX */
		/* First we have to qualify that the ACKed value is within our window */
		for (x=session->rseqno; x != session->oseqno; x++)
			if (fh->iseqno == x)
				break;
		if ((x != session->oseqno) || (session->oseqno == fh->iseqno))
		{
			/* The acknowledgement is within our window.  Time to acknowledge everything
				that it says to */
			for (x=session->rseqno; x != fh->iseqno; x++)
			{
				/* Ack the packet with the given timestamp */
				DEBU(G "Cancelling transmission of packet %d\n", x);
				sch = schedq;
				while(sch)
				{
					if ( sch->frame &&
					     sch->frame->session == session &&
					     sch->frame->oseqno == x
					   )
						sch->frame->retries = -1;
					sch = sch->next;
				}
			}
			/* Note how much we've received acknowledgement for */
			session->rseqno = fh->iseqno;
		} else
			DEBU(G "Received iseqno %d not within window %d->%d\n", fh->iseqno, session->rseqno, session->oseqno);
	}

	/* Check where we are */
	if ((ntohs(fh->dcallno) & IAX_FLAG_RETRANS) ||
			(fh->type != AST_FRAME_VOICE &&
			 fh->type != AST_FRAME_VIDEO))
		updatehistory = 0;

	if (session->iseqno != fh->oseqno &&
		(session->iseqno || fh->type != AST_FRAME_IAX ||
		 (subclass != IAX_COMMAND_TXREADY &&
		  subclass != IAX_COMMAND_TXREL &&
		  subclass != IAX_COMMAND_TXCNT &&
		  subclass != IAX_COMMAND_TXACC)))
	{
		if (fh->type != AST_FRAME_IAX ||
				(subclass != IAX_COMMAND_ACK &&
				 subclass != IAX_COMMAND_INVAL &&
				 subclass != IAX_COMMAND_TXREADY &&
				 subclass != IAX_COMMAND_TXREL &&
				 subclass != IAX_COMMAND_TXCNT &&
				 subclass != IAX_COMMAND_TXACC &&
				 subclass != IAX_COMMAND_VNAK))
		{
			/* If it's not an ACK packet, it's out of order. */
			DEBU(G "Packet arrived out of order (expecting %d, got %d) (frametype = %d, subclass = %d)\n",
				session->iseqno, fh->oseqno, fh->type, subclass);
			
			/* 
			 * Check if session->iseqno > fh->oseqno, accounting for possible wrap around
			 * This is correct if the two values are not equal (which, in this case, is guaranteed)
			 */
			if ( (unsigned char)(session->iseqno - fh->oseqno) < 128 )
			{
				/* If we've already seen it, ack it XXX There's a border condition here XXX */
				if ((fh->type != AST_FRAME_IAX) ||
						(subclass != IAX_COMMAND_ACK &&
						 subclass != IAX_COMMAND_INVAL))
				{
					DEBU(G "Acking anyway\n");
					/* XXX Maybe we should handle its ack to us, but then again, it's probably outdated anyway, and if
						we have anything to send, we'll retransmit and get an ACK back anyway XXX */
					send_command_immediate(session, AST_FRAME_IAX, IAX_COMMAND_ACK, ts, NULL, 0,fh->iseqno);
				}
			}
			else
			{
				/* Send a VNAK requesting retransmission */
				iax2_vnak(session);
			}
			return NULL;
		}
	}
	else
	{
		/* Increment unless it's an ACK or VNAK */
		if (fh->type != AST_FRAME_IAX ||
				(subclass != IAX_COMMAND_ACK &&
				 subclass != IAX_COMMAND_INVAL &&
				 subclass != IAX_COMMAND_TXCNT &&
				 subclass != IAX_COMMAND_TXACC &&
				 subclass != IAX_COMMAND_VNAK))
			session->iseqno++;
	}

	e = calloc(1, sizeof(struct iax_event) + datalen + 1);

	if (!e) {
		DEBU(G "Out of memory\n");
		return NULL;
	}

	/* Set etype to some unknown value so do not inavertently
	 * sending IAX_EVENT_CONNECT event, which is 0 to application.
	 */
	e->etype = -1;
	e->session = session;
	switch(fh->type) {
	case AST_FRAME_DTMF:
		e->etype = IAX_EVENT_DTMF;
		e->subclass = subclass;
		/*
		 We want the DTMF event deliver immediately so all I/O can be
		 terminate quickly in an IVR system.
		e = schedule_delivery(e, ts, updatehistory); */
		break;
	case AST_FRAME_VOICE:
		e->etype = IAX_EVENT_VOICE;
		e->subclass = subclass;
		e->ts = ts;
		session->voiceformat = subclass;
		if (datalen) {
			memcpy(e->data, fh->iedata, datalen);
			e->datalen = datalen;
		}
		e = schedule_delivery(e, ts, updatehistory);
		break;
	case AST_FRAME_CNG:
		e->etype = IAX_EVENT_CNG;
		e->subclass = subclass;
		if (datalen) {
			memcpy(e->data, fh->iedata, datalen);
			e->datalen = datalen;
		}
		e = schedule_delivery(e, ts, updatehistory);
		break;
	case AST_FRAME_IAX:
		/* Parse IE's */
		if (datalen) {
			memcpy(e->data, fh->iedata, datalen);
			e->datalen = datalen;
		}
		if (iax_parse_ies(&e->ies, e->data, e->datalen)) {
			IAXERROR "Unable to parse IE's");
			free(e);
			e = NULL;
			break;
		}
		switch(subclass) {
		case IAX_COMMAND_NEW:
			/* This is a new, incoming call */
			/* save the capability for validation */
			session->capability = e->ies.capability;
			if (e->ies.codec_prefs) {
				strncpy(session->codec_order,
						e->ies.codec_prefs,
						sizeof(session->codec_order));
				session->codec_order_len =
					(int)strlen(session->codec_order);
			}
			e->etype = IAX_EVENT_CONNECT;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_AUTHREQ:
			/* This is a request for a call */
			e->etype = IAX_EVENT_AUTHRQ;
			if (strlen(session->username) && !strcmp(e->ies.username, session->username) &&
				strlen(session->secret)) {
					/* Hey, we already know this one */
					iax_auth_reply(session, session->secret, e->ies.challenge, e->ies.authmethods);
					free(e);
					e = NULL;
					break;
			}
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_HANGUP:
			e->etype = IAX_EVENT_HANGUP;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_INVAL:
			e->etype = IAX_EVENT_HANGUP;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_REJECT:
			e->etype = IAX_EVENT_REJECT;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_ACK:
			free(e);
			e = NULL;
			break;
		case IAX_COMMAND_VNAK:
			iax_handle_vnak(session, fh);
			free(e);
			e = NULL;
			break;
		case IAX_COMMAND_LAGRQ:
			/* Pass this along for later handling */
			e->etype = IAX_EVENT_LAGRQ;
			e->ts = ts;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_POKE:
			e->etype = IAX_EVENT_POKE;
			e->ts = ts;
			break;
		case IAX_COMMAND_PING:
			/* PINGS and PONGS don't get scheduled; */
			e->etype = IAX_EVENT_PING;
			e->ts = ts;
			break;
		case IAX_COMMAND_PONG:
			e->etype = IAX_EVENT_PONG;
			/* track weighted average of ping time */
			session->pingtime = ((2 * session->pingtime) + (calc_timestamp(session, 0, 0) - ts)) / 3;
			session->remote_netstats.jitter = e->ies.rr_jitter;
			session->remote_netstats.losspct = e->ies.rr_loss >> 24;;
			session->remote_netstats.losscnt = e->ies.rr_loss & 0xffffff;
			session->remote_netstats.packets = e->ies.rr_pkts;
			session->remote_netstats.delay = e->ies.rr_delay;
			session->remote_netstats.dropped = e->ies.rr_dropped;
			session->remote_netstats.ooo = e->ies.rr_ooo;
			break;
		case IAX_COMMAND_ACCEPT:
			if (e->ies.format & session->capability) {
				e->etype = IAX_EVENT_ACCEPT;
			} else {
				struct iax_ie_data ied;
				/* Although this should not happen, we
				 * added this to make sure the
				 * negotiation protocol is enforced.
				 * For lack of event to notify the
				 * application we use the defined
				 * REJECT event.
				 */
				memset(&ied, 0, sizeof(ied));
				iax_ie_append_str(&ied, IAX_IE_CAUSE, "Unable to negotiate codec");
				send_command_final(session, AST_FRAME_IAX, IAX_COMMAND_REJECT, 0, ied.buf, ied.pos);
				e->etype = IAX_EVENT_REJECT;
			}
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_REGACK:
			e->etype = IAX_EVENT_REGACK;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_REGAUTH:
			iax_regauth_reply(session, session->secret, e->ies.challenge, e->ies.authmethods);
			free(e);
			e = NULL;
			break;
		case IAX_COMMAND_REGREJ:
			e->etype = IAX_EVENT_REGREJ;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_LAGRP:
			e->etype = IAX_EVENT_LAGRP;
			nowts = calc_timestamp(session, 0, 0);
			e->ts = nowts - ts;
			/* Can't call schedule_delivery since timestamp is non-normal */
			break;;
		case IAX_COMMAND_TXREQ:
			/* added check for defensive programming
			 * - in case the asterisk server
			 * or another client does not send the
			 *  apparent transfer address
			 */
			if (e->ies.apparent_addr != NULL) {
				/* so a full voice frame is sent on the
				   next voice output */
				session->svoiceformat = -1;
				session->transfer = *e->ies.apparent_addr;
				session->transfer.sin_family = AF_INET;
				session->transfercallno = e->ies.callno;
				session->transferring = TRANSFER_BEGIN;
				session->transferid = e->ies.transferid;
				iax_send_txcnt(session);
			}
			free(e);
			e = NULL;
			break;
		case IAX_COMMAND_DPREP:
			/* Received dialplan reply */
			e->etype = IAX_EVENT_DPREP;
			/* Return immediately, makes no sense to schedule */
			break;
		case IAX_COMMAND_TXCNT:
			if (session->transferring)  {
				session->transfer = *sin;
				iax_send_txaccept(session);
			}
			free(e);
			e = NULL;
			break;
		case IAX_COMMAND_TXACC:
			if (session->transferring) {
				stop_transfer(session);
				session->transferring = TRANSFER_READY;
				iax_send_txready(session);
			}
			free(e);
			e = NULL;
			break;
		case IAX_COMMAND_TXREL:
			/* Release the transfer */
			send_command_immediate(session, AST_FRAME_IAX, IAX_COMMAND_ACK, ts, NULL, 0, fh->iseqno);
			if (session->transferring) {
				complete_transfer(session, e->ies.callno, 1, 0);
			} else {
				complete_transfer(session, session->peercallno, 0, 1);
			}
			e->etype = IAX_EVENT_TRANSFER;
			/* notify that asterisk no longer sitting between peers */
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case IAX_COMMAND_QUELCH:
			e->etype = IAX_EVENT_QUELCH;
			session->quelch = 1;
			break;
		case IAX_COMMAND_UNQUELCH:
			e->etype = IAX_EVENT_UNQUELCH;
			session->quelch = 0;
			break;
		case IAX_COMMAND_TXREJ:
			e->etype = IAX_EVENT_TXREJECT;
			iax_handle_txreject(session);
			break;

		case IAX_COMMAND_TXREADY:
			send_command_immediate(session, AST_FRAME_IAX, IAX_COMMAND_ACK, ts, NULL, 0, fh->iseqno);
			if (iax_handle_txready(session)) {
				e->etype = IAX_EVENT_TXREADY;
			} else {
				free(e);
				e = NULL;
			}
			break;
		default:
			DEBU(G "Don't know what to do with IAX command %d\n", subclass);
			free(e);
			e = NULL;
		}
		break;
	case AST_FRAME_CONTROL:
		switch(subclass) {
		case AST_CONTROL_ANSWER:
			e->etype = IAX_EVENT_ANSWER;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case AST_CONTROL_CONGESTION:
		case AST_CONTROL_BUSY:
			e->etype = IAX_EVENT_BUSY;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case AST_CONTROL_RINGING:
			e->etype = IAX_EVENT_RINGA;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case AST_CONTROL_KEY:
			e->etype = IAX_EVENT_KEY;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case AST_CONTROL_UNKEY:
			e->etype = IAX_EVENT_UNKEY;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		default:
			DEBU(G "Don't know what to do with AST control %d\n", subclass);
			free(e);
			return NULL;
		}
		break;
	case AST_FRAME_IMAGE:
		e->etype = IAX_EVENT_IMAGE;
		e->subclass = subclass;
		if (datalen) {
			memcpy(e->data, fh->iedata, datalen);
		}
		e = schedule_delivery(e, ts, updatehistory);
		break;
	case AST_FRAME_VIDEO:
		e->etype = IAX_EVENT_VIDEO;
		e->subclass = subclass;
		e->ts = ts;
		session->videoformat = e->subclass;
		memcpy(e->data, fh->iedata, datalen);
		e->datalen = datalen;
		e = schedule_delivery(e, ts, updatehistory);
		break;
	case AST_FRAME_TEXT:
		e->etype = IAX_EVENT_TEXT;
		if (datalen) {
			memcpy(e->data, fh->iedata, datalen);
			e->datalen = datalen;
		}
		e = schedule_delivery(e, ts, updatehistory);
		break;

	case AST_FRAME_HTML:
		switch(fh->csub) {
		case AST_HTML_LINKURL:
			e->etype = IAX_EVENT_LINKURL;
			/* Fall through */
		case AST_HTML_URL:
			if (e->etype == -1)
				e->etype = IAX_EVENT_URL;
			e->subclass = fh->csub;
			e->datalen = datalen;
			if (datalen) {
				memcpy(e->data, fh->iedata, datalen);
			}
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case AST_HTML_LDCOMPLETE:
			e->etype = IAX_EVENT_LDCOMPLETE;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case AST_HTML_UNLINK:
			e->etype = IAX_EVENT_UNLINK;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		case AST_HTML_LINKREJECT:
			e->etype = IAX_EVENT_LINKREJECT;
			e = schedule_delivery(e, ts, updatehistory);
			break;
		default:
			DEBU(G "Don't know how to handle HTML type %d frames\n", fh->csub);
			free(e);
			return NULL;
		}
		break;
	default:
		DEBU(G "Don't know what to do with frame type %d\n", fh->type);
		free(e);
		return NULL;
	}

	/* Already ack'd iax frames */
	if (session->aseqno != session->iseqno) {
		send_command_immediate(session, AST_FRAME_IAX, IAX_COMMAND_ACK, ts, NULL, 0, fh->iseqno);
	}
	return e;
}

/* Some parts taken from iax_miniheader_to_event and from from chan_iax2.c. We must inform Mark Spencer? */
static struct iax_event *iax_videoheader_to_event(struct iax_session *session,
		struct ast_iax2_video_hdr *vh, int datalen)
{
	struct iax_event * e;

	if ( session->videoformat <= 0 )
	{
		DEBU(G "No last video format received on session %d\n",
				session->callno);
		return 0;
	}

	e = calloc(1, sizeof(struct iax_event) + datalen);

	if ( !e )
	{
		DEBU(G "Out of memory\n");
		return 0;
	}

	e->etype = IAX_EVENT_VIDEO;
	e->session = session;
	e->subclass = session->videoformat | (ntohs(vh->ts) & 0x8000 ? 1 : 0);
	e->datalen = datalen;
	memcpy(e->data, vh->data, e->datalen);
	e->ts = (session->last_ts & 0xFFFF8000L) | (ntohs(vh->ts) & 0x7fff);

	return schedule_delivery(e, e->ts, 1);
}

static struct iax_event *iax_miniheader_to_event(struct iax_session *session,
		struct ast_iax2_mini_hdr *mh, int datalen)
{
	struct iax_event * e;

	if ( session->voiceformat <= 0 )
	{
		DEBU(G "No last format received on session %d\n", session->callno);
		return 0;
	}

	e = calloc(1, sizeof(struct iax_event) + datalen);

	if ( !e )
	{
		DEBU(G "Out of memory\n");
		return 0;
	}

	e->etype = IAX_EVENT_VOICE;
	e->session = session;
	e->subclass = session->voiceformat;
	e->datalen = datalen;
	memcpy(e->data, mh->data, datalen);
	e->ts = (session->last_ts & 0xFFFF0000) | ntohs(mh->ts);

	return schedule_delivery(e, e->ts, 1);
}

void iax_destroy(struct iax_session *session)
{
	destroy_session(session);
}

static struct iax_event *iax_net_read(void)
{
	unsigned char buf[65536];
	int res;
	struct sockaddr_in sin;
	socklen_t sinlen;
	struct iax_event *event;

	sinlen = sizeof(sin);
	res = iax_recvfrom(netfd, (char *)buf, sizeof(buf), 0, (struct sockaddr *) &sin, &sinlen);
	if (res < 0) {
#if defined(_WIN32_WCE)
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			DEBU(G "Error on read: %d\n", WSAGetLastError());
			IAXERROR "Read error on network socket: ???");
		}
#elif defined(WIN32)  ||  defined(_WIN32_WCE)
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			DEBU(G "Error on read: %d\n", WSAGetLastError());
			IAXERROR "Read error on network socket: %s", strerror(errno));
		}
#else
		if (errno != EAGAIN) {
			DEBU(G "Error on read: %s\n", strerror(errno));
			IAXERROR "Read error on network socket: %s", strerror(errno));
		}
#endif
		return NULL;
	}
	event = iax_net_process(buf, res, &sin);
	if ( event == NULL )
	{
		// We have received a frame. The corresponding event is queued
		// We need to motify the entire stack of calling functions so they
		// don't go to sleep thinking there are no more frames to process
		// TODO: this is buttugly from a design point of view. Basically we
		// change libiax2 behavior to accomodate iaxclient.
		// There must be a way to do it better.
		event = calloc(1, sizeof(struct iax_event));
		if ( event != NULL )
			event->etype = IAX_EVENT_NULL;
	}
	return event;
}

static struct iax_session *iax_txcnt_session(struct ast_iax2_full_hdr *fh, int datalen,
				struct sockaddr_in *sin, short callno, short dcallno)
{
	int subclass = uncompress_subclass(fh->csub);
	unsigned char buf[ 65536 ]; /* allocated on stack with same size as iax_net_read() */
	struct iax_ies ies;
	struct iax_session *cur;

	if ((fh->type != AST_FRAME_IAX) || (subclass != IAX_COMMAND_TXCNT) || (!datalen)) {
		return NULL; /* special handling for TXCNT only */
	}
	memcpy(buf, fh->iedata, datalen);	/* prepare local buf for iax_parse_ies() */

	if (iax_parse_ies(&ies, buf, datalen)) {
		return NULL;	/* Unable to parse IE's */
	}
	if (!ies.transferid) {
		return NULL;	/* TXCNT without proper IAX_IE_TRANSFERID */
	}
	for( cur=sessions; cur; cur=cur->next ) {
		if ((cur->transferring) && (cur->transferid == (int) ies.transferid) &&
		   	(cur->callno == dcallno) && (cur->transfercallno == callno)) {
			/* We're transferring ---
			 *  skip address/port checking which would fail while
			 *  remote peer behind symmetric NAT, verify
			 *  transferid instead
			 */
			cur->transfer.sin_addr.s_addr = sin->sin_addr.s_addr; /* setup for further handling */
			cur->transfer.sin_port = sin->sin_port;
			break;
		}
	}
	return cur;
}

struct iax_event *iax_net_process(unsigned char *buf, int len, struct sockaddr_in *sin)
{
	struct ast_iax2_full_hdr *fh = (struct ast_iax2_full_hdr *)buf;
	struct ast_iax2_mini_hdr *mh = (struct ast_iax2_mini_hdr *)buf;
	struct ast_iax2_video_hdr *vh = (struct ast_iax2_video_hdr *)buf;
	struct iax_session *session;

	if ((size_t)len < sizeof(fh->scallno)) {
		DEBU(G "Short header received from %s\n", inet_ntoa(sin->sin_addr));
		IAXERROR "Short header received from %s\n", inet_ntoa(sin->sin_addr));
		return NULL;
	}

	if (ntohs(fh->scallno) & IAX_FLAG_FULL) {
		/* Full size header */
		if ((size_t)len < sizeof(struct ast_iax2_full_hdr)) {
			DEBU(G "Short header received from %s\n", inet_ntoa(sin->sin_addr));
			IAXERROR "Short header received from %s\n", inet_ntoa(sin->sin_addr));
			return NULL;
		}
		/* We have a full header, process appropriately */
		session = iax_find_session(sin,
				ntohs(fh->scallno) & ~IAX_FLAG_FULL,
				ntohs(fh->dcallno) & ~IAX_FLAG_RETRANS, 1);
		if (!session)
			session = iax_txcnt_session(fh,
					len - sizeof(struct ast_iax2_full_hdr),
					sin, ntohs(fh->scallno) & ~IAX_FLAG_FULL,
					ntohs(fh->dcallno) & ~IAX_FLAG_RETRANS);
		if (session)
			return iax_header_to_event(session, fh, len - sizeof(struct ast_iax2_full_hdr), sin);
	} else {
		if ((size_t)len < sizeof(struct ast_iax2_mini_hdr)) {
			DEBU(G "Short header received from %s\n", inet_ntoa(sin->sin_addr));
			IAXERROR "Short header received from %s\n", inet_ntoa(sin->sin_addr));
			return NULL;
		}

		if (mh->callno == 0) {
			/* We have a meta frame, could be a video meta frame
			 * or an ordinary meta frame, to find out we check
			 * the V flag.
			 */
			if (!(ntohs(vh->callno) & IAX_FLAG_VIDEO)) {
				DEBU(G "Meta frame received from %s, but we cannot handle it\n",
						inet_ntoa(sin->sin_addr));
				IAXERROR "Meta frame received from %s, but we cannot handle it\n",
						inet_ntoa(sin->sin_addr));
				return NULL;
			}
			/* it is a video metaframe, verify its size */
			if ((size_t)len < sizeof(struct ast_iax2_video_hdr)) {
				DEBU(G "Short video mini header received from %s\n",
						inet_ntoa(sin->sin_addr));
				IAXERROR "Short video mini header received from %s\n",
						inet_ntoa(sin->sin_addr));
				return NULL;
			}

			session = iax_find_session(sin, ntohs(vh->callno) & ~IAX_FLAG_VIDEO, 0, 0);

			if (session)
				return iax_videoheader_to_event(session, vh,
						len - sizeof(struct ast_iax2_video_hdr));
		} else {
			/* mini audio frame */
			session = iax_find_session(sin, ntohs(mh->callno), 0, 0);
			if (session)
				return iax_miniheader_to_event(session, mh,
						len - sizeof(struct ast_iax2_mini_hdr));
		}
	}
	DEBU(G "No session?\n");
	return NULL;
}

static struct iax_sched *iax_get_sched(struct timeval tv)
{
	/* Just need to check the head of schedq because the earliest item is
	 * guaranteed to be first. */
	if (schedq && iax_tvcmp(tv, schedq->when) >= 0) {
		struct iax_sched *tmp = schedq;
		schedq = tmp->next;
		return tmp;
	}
	return NULL;
}

struct iax_event *iax_get_event(int blocking)
{
	struct timeval tv = iax_tvnow();
	struct iax_sched *cur;
	struct iax_session *session;

	while ((cur = iax_get_sched(tv)))
	{
		struct iax_event *event = cur->event;
		struct iax_frame *frame = cur->frame;

		if (event)
		{
			/* See if this is an event we need to handle */
			event = handle_event(event);
			if (event)
			{
				free(cur);
				return event;
			}
		}
		else if (frame)
		{
			/* It's a frame, transmit it and schedule a retry */
			if (frame->retries < 0)
			{
				/* It's been acked.  No need to send it.   Destroy the old
				   frame. If final, destroy the session. */
				if (frame->final)
					destroy_session(frame->session);
				iax_frame_free(frame);
			}
			else if (frame->retries == 0)
			{
				if (frame->transfer)
				{
					/* Send a transfer reject since we weren't able to connect */
					iax_send_txrej(frame->session);
					iax_frame_free(frame);
					free(cur);
					break;
				}
				else
				{
					/* We haven't been able to get an ACK on this packet. If a
					   final frame, destroy the session, otherwise, pass up timeout */
					if (frame->final)
					{
						destroy_session(frame->session);
						iax_frame_free(frame);
					}
					else
					{
						event = calloc(1, sizeof(struct iax_event));
						if (event)
						{
							event->etype = IAX_EVENT_TIMEOUT;
							event->session = frame->session;
							iax_frame_free(frame);
							free(cur);
							return handle_event(event);
						}
					}
				}
			}
			else
			{
				struct ast_iax2_full_hdr *fh = frame->rawdata;

				/* Decrement remaining retries */
				frame->retries--;

				/* Multiply next retry time by 4, not above
				 * MAX_RETRY_TIME though */
				frame->retrytime *= 4;

				if (frame->transfer)
				{
					if (frame->retrytime > 1000)
						frame->retrytime = 1000;
				}
				else
				{
					if (frame->retrytime > MAX_RETRY_TIME)
						frame->retrytime = MAX_RETRY_TIME;
				}
				fh->dcallno = htons(IAX_FLAG_RETRANS | frame->dcallno);
				iax_xmit_frame(frame);

				/* Schedule another retransmission */
				DEBU(G "Scheduling retransmission %d\n", frame->retries);
				iax_sched_add(NULL, frame, NULL, NULL, frame->retrytime);
			}
		}
		else if (cur->func)
		{
		    cur->func(cur->arg);
		}
		free(cur);
	}

	/* get jitterbuffer-scheduled events */
	for ( session = sessions; session; session = session->next )
	{
		int ret;
		long now;
		long next;
		jb_frame frame;
		struct iax_event * event;

		now = iax_tvdiff_ms(tv, session->rxcore);

		if ( now <= (next = jb_next(session->jb)) )
			continue;

		/* interp len no longer hardcoded, now determined by
		 * get_interp_len */
		ret = jb_get(session->jb, &frame, now,
				get_interp_len(session->voiceformat));

		switch (ret) {
		case JB_OK:
			event = (struct iax_event *)frame.data;
			event = handle_event(event);
			if (event)
				return event;
			break;
		case JB_INTERP:
			/* create an interpolation frame */
			//fprintf(stderr, "Making Interpolation frame\n");
			event = calloc(1, sizeof(struct iax_event));
			if (event) {
				event->etype    = IAX_EVENT_VOICE;
				event->subclass = session->voiceformat;
				/* XXX: ??? applications probably ignore this anyway */
				event->ts       = now;
				event->session  = session;
				event->datalen  = 0;
				event = handle_event(event);
				if(event)
					return event;
			}
			break;
		case JB_DROP:
			iax_event_free((struct iax_event *)frame.data);
			break;
		case JB_NOFRAME:
		case JB_EMPTY:
			/* do nothing */
			break;
		default:
			/* shouldn't happen */
			break;
		}
	}

	/* Now look for networking events */
	if (blocking) {
		/* Block until there is data if desired */
		fd_set fds;
		int next_event_time;

		FD_ZERO(&fds);
		FD_SET(netfd, &fds);

		next_event_time = iax_time_to_next_event();

		if (next_event_time < 0) {
			select(netfd + 1, &fds, NULL, NULL, NULL);
		} else {
			struct timeval next_event;

			next_event.tv_sec = next_event_time / 1000;
			next_event.tv_usec = (next_event_time % 1000) * 1000;

			select(netfd + 1, &fds, NULL, NULL, &next_event);
		}

	}

	return handle_event(iax_net_read());
}

struct sockaddr_in iax_get_peer_addr(struct iax_session *session)
{
	return session->peeraddr;
}

void iax_session_destroy(struct iax_session **session)
{
	destroy_session(*session);
	*session = NULL;
}

void iax_event_free(struct iax_event *event)
{
	/* We gave the user a chance to play with the session now we need to
	 * destroy it if you are not calling this function on every event you
	 * read you are now going to leak sessions as well as events!
	 */
	switch(event->etype) {
	case IAX_EVENT_REJECT:
	case IAX_EVENT_HANGUP:
		/* Destroy this session -- it's no longer valid */
		if (event->session) { /* maybe the user did it already */
			destroy_session(event->session);
		}
		break;
	}
	free(event);
}

int iax_get_fd(void)
{
	/* Return our network file descriptor. The client can select on this
	 * (probably with other things, or can add it to a network add sort
	 * of gtk_input_add for example */
	return netfd;
}

int iax_quelch_moh(struct iax_session *session, int MOH)
{
	struct iax_ie_data ied;			//IE Data Structure (Stuff To Send)
	memset(&ied, 0, sizeof(ied));

	// You can't quelch the quelched
	if (session->quelch == 1)
		return -1;

	if (MOH) {
		iax_ie_append(&ied, IAX_IE_MUSICONHOLD);
		session->transfer_moh = 1;
	}

	return send_command(session, AST_FRAME_IAX, IAX_COMMAND_QUELCH, 0, ied.buf, ied.pos);
}

struct timeval iax_tvnow(void)
{
	struct timeval tv;

#ifdef HAVE_GETTIMEOFDAY
	gettimeofday(&tv, 0);
#elif defined(_MSC_VER)
	struct _timeb curSysTime;

	_ftime(&curSysTime);
	tv.tv_sec = (long)curSysTime.time;
	tv.tv_usec = curSysTime.millitm * 1000;
#else
#error no gettimeofday or equivalent available
#endif
	return tv;
}
