#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <oggz/oggz.h>
#include <theora/theora.h>

#include "file.h"

#ifdef __GNUC__
void mylog(const char * fmt, ...) __attribute__ ((format (printf, 1, 2)));
#else
void mylog(const char * fmt, ...);
#endif

#define THEORA_FRAME_DURATION 1000 / 15

struct op_node
{
	ogg_packet     *op;
	long           serialno;
	long           timestamp;
	struct op_node *next;
};

struct theora_headers
{
	theora_info    ti;
	theora_comment tc;
	theora_state   ts;
	int            header_count;
	int            have_headers;
};

struct ogg_stream
{
	struct op_node *first;
	struct op_node *last;
	struct op_node *current;
	long           serialno;
	long           page_ts;
	long           page_count;
	long           base_ts;
	void           *data;
};

static struct ogg_stream *audio_stream;
static struct ogg_stream *video_stream;

static struct op_node *
create_node(ogg_packet *op, long serialno, long timestamp)
{
	struct op_node *node;

	node = malloc(sizeof(struct op_node));
	node->timestamp = timestamp;
	node->serialno = serialno;
	node->op = malloc(sizeof(*op));
	memcpy(node->op, op, sizeof(*op));
	node->op->packet = malloc(op->bytes);
	memcpy(node->op->packet, op->packet, op->bytes);

	return node;
}

static void
append_node(struct ogg_stream *os, struct op_node *node)
{
	if ( os->first == NULL )
	{
		if ( os->last != NULL )
		{
			mylog("Queue inconsistency, bailing...\n");
			return;
		}
		os->first = node;
		os->last = node;
		node->next = NULL;
	} else
	{
		if ( os->last == NULL )
		{
			mylog("Queue inconsistency, bailing...\n");
			return;
		}
		os->last->next = node;
		os->last = node;
		node->next = NULL;
	}
}

/*
 * We're forced to use a dirty hack here, due to Theora's idiotic API
 * Theora needs three separate pieces of data, called headers to initialize
 * its internal decoder structure.  After all three pieces have been received,
 * we can call theora_decode_init.
 * We use a counter and a flag to make sure we have decoded our three headers and then
 * we call theora_decode_init so we can initialize a theora_state structure.
 * We use the ts structure to convert a granule position into an actual timestamp.
 * There are many ways in which this can fail, but we rely on having all three headers
 * at the beginning of the ogg video bitstream.
 *
 * To whoever came up with this convoluted scheme: please consider a change of careers.
 */
static int
read_theora_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data)
{
	struct op_node        *node;
	struct theora_headers *th;
	long                  timestamp = 0;

	th = (struct theora_headers *)video_stream->data;

	if ( theora_packet_isheader(op) )
	{
		theora_decode_header(&(th->ti), &(th->tc), op);
		th->header_count++;
	}

	if ( th->header_count >= 3 && !th->have_headers )
	{
		theora_decode_init(&(th->ts), &(th->ti));
		th->have_headers = 1;
	}

	if ( th->have_headers )
	{
		double d;

		d = theora_granule_time(&(th->ts), op->granulepos);
		timestamp = (long)(d * 1000);
	}

	if ( timestamp < 0 )
	{
		timestamp = video_stream->page_ts +
			video_stream->page_count * THEORA_FRAME_DURATION;
		video_stream->page_count++;
	} else
	{
		video_stream->page_ts = timestamp;
		video_stream->page_count = 0;
	}

	if ( !theora_packet_isheader(op) )
	{
		node = create_node(op, serialno, timestamp);
		append_node(video_stream, node);
	}

	return 0;
}

static int
read_speex_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data)
{
	static int cnt = 0;
	const long timestamp = audio_stream->page_ts +
		audio_stream->page_count * SPEEX_FRAME_DURATION;

	audio_stream->page_count++;

	cnt++;

	// Ignore the first two packets, they are headers
	if ( cnt > 2 )
	{
		struct op_node *node = create_node(op, serialno, timestamp);
		append_node(audio_stream, node);
	}

	return 0;
}

static int
read_cb(OGGZ *oggz, ogg_packet *op, long serialno, void *data)
{
	struct theora_headers *th;

	const char theoraId[] = "\x80theora";
	const char speexId[] = "Speex   ";

	if ( memcmp(op->packet, theoraId, strlen(theoraId)) == 0 )
	{
		oggz_set_read_callback(oggz, serialno, read_theora_cb, NULL);
		video_stream->serialno = serialno;

		// Initialize theora specific data fields
		th = calloc(1, sizeof(struct theora_headers));
		theora_info_init(&(th->ti));
		theora_comment_init(&(th->tc));
		video_stream->data = th;

		read_theora_cb(oggz, op, serialno, data);
	} else if ( memcmp(op->packet, speexId, strlen(speexId)) == 0 )
	{
		oggz_set_read_callback(oggz, serialno, read_speex_cb, NULL);
		audio_stream->serialno = serialno;
		read_speex_cb(oggz, op, serialno, data);
	} else
	{
		mylog("Got unknown ogg packet, serialno=%ld, size=%ld, "
				"packetno=%lld, granulepos=%lld\n",
				serialno, op->bytes,
				op->packetno, op->granulepos);
	}
	return 0;
}

static int
read_page_cb(OGGZ *oggz, const ogg_page *og, long serialno, void *data)
{
	if ( serialno == audio_stream->serialno )
	{
		audio_stream->page_ts =
			ogg_page_granulepos((ogg_page *)og) * 1000 /
			SPEEX_SAMPLING_RATE;
		audio_stream->page_count = 0;
	}
	else if ( serialno == video_stream->serialno )
	{
	}
	return 0;
}

int
load_ogg_file(const char *filename)
{
	OGGZ *oggz;

	oggz = oggz_open(filename, OGGZ_READ | OGGZ_AUTO);
	if ( oggz == NULL )
	{
		mylog("Error opening ogg file\n");
		return -1;
	}
	mylog("Successfully opened ogg file %s\n", filename);

	// Initialize internal streams
	audio_stream = calloc(1, sizeof(struct ogg_stream));
	video_stream = calloc(1, sizeof(struct ogg_stream));

	oggz_set_read_callback(oggz, -1, read_cb, NULL);
	oggz_set_read_page(oggz, -1, read_page_cb, NULL);

	oggz_run(oggz);

	oggz_close(oggz);

	return 0;
}

static ogg_packet * get_next_op(struct ogg_stream *os, struct timeval tv)
{
	ogg_packet *op = 0;
	long time_now;

	if ( !os )
		return NULL;

	time_now = tv.tv_sec * 1000 + tv.tv_usec / 1000;

	if ( !os->current )
	{
		if ( !os->first )
			return NULL;

		// point to the beginning of the stream and reset the time base
		os->base_ts = time_now;
		os->current = os->first;
	}

	if ( os->current->timestamp < time_now - os->base_ts )
	{
		op = os->current->op;
		os->current = os->current->next;
	}

	return op;
}

ogg_packet * get_next_audio_op(struct timeval now)
{
	return get_next_op(audio_stream, now);
}

ogg_packet * get_next_video_op(struct timeval now)
{
	return get_next_op(video_stream, now);
}

int audio_is_eos()
{
	return audio_stream->current == NULL;
}

int video_is_eos()
{
	return video_stream->current == NULL;
}

