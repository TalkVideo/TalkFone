/*
* stresstest: simple program for applying IAX2 protocol stress to asterisk.
*
* Copyrights:
* Copyright (C) 2007, Wimba, Inc.
*
* Contributors:
* Mihai Balea <mihai AT hates DOT ms>
* Peter Grayson <jpgrayson@gmail.com>
*
* This program is free software, distributed under the terms of
* the GNU Lesser (Library) General Public License.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>

#include "iaxclient.h"
#include "file.h"

#ifdef WIN32
#include <process.h>
typedef uintptr_t a_thread;
#define STDCALL	__stdcall
#define snprintf _snprintf
#undef main
#else
#include <pthread.h>
typedef pthread_t a_thread;
#define STDCALL
#endif

static const char STRESS_CMD_UNMUTE_AUDIO[] = "unmute_audio";
static const char STRESS_CMD_MUTE_AUDIO[] = "mute_audio";
static const char STRESS_CMD_UNMUTE_VIDEO[] = "unmute_video";
static const char STRESS_CMD_MUTE_VIDEO[] = "mute_video";
static const char STRESS_CMD_SHOW_STATE[] = "show_state";
static const char STRESS_CMD_QUIT[] = "quit";

enum
{
	TEST_OK = 0,
	TEST_NO_CONNECTION = -1,
	TEST_NO_MEDIA = -2,
	TEST_UNKNOWN_ERROR = -99,
};

static const int format =
		IAXC_FORMAT_H263 |
		IAXC_FORMAT_H263_PLUS |
		IAXC_FORMAT_H264 |
		IAXC_FORMAT_MPEG4 |
		IAXC_FORMAT_THEORA;
static int formatp = IAXC_FORMAT_THEORA;
static int framerate = 15;
static int bitrate = 200000;
static int width = 320;
static int height = 240;
static int fragsize = 1400;

static int call_established = 0;
static int running = 1;

static int camera_enabled_sent = 0;
static int send_video = 1;
static int send_audio = 1;
static int print_netstats = 0;
static int call_timeout_ms = 0;
static int connect_timeout_ms = 5000;
static int video_frames_count = 0;
static int audio_frames_count = 0;
static int audio_sent_count = 0;

static struct timeval start_time;

static FILE * log_file = 0;

#ifdef __GNUC__
void mylog(const char * fmt, ...) __attribute__ ((format (printf, 1, 2)));
#endif

void mylog(const char * fmt, ...)
{
	va_list ap;
	time_t t;
	struct tm * tmp;
	char str[1024];
	char time_str[1024];

	t = time(0);
	tmp = localtime(&t);

	strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tmp);
	snprintf(str, sizeof(str), "stresstest: %s: %s", time_str, fmt);

	va_start(ap, fmt);
	vfprintf(log_file ? log_file : stderr, str, ap);
	va_end(ap);
}

static int
create_thread(a_thread *thread,
		unsigned int (STDCALL * thread_func)(void *),
		void *args, unsigned int *thread_id)
{
	int ret = 0;
#ifdef WIN32
	*thread = (uintptr_t)_beginthreadex(NULL, 0, thread_func,
			(void *)args, 0, thread_id);

	if ( thread == 0 )
		ret = errno;
#else
	void * (*func)(void *) = (void * (*)(void *))thread_func;

	ret = pthread_create(thread, NULL, func, args);
#endif

	return ret;
}

static struct timeval
get_now(void)
{
	struct timeval tv;
#ifdef WIN32
	FILETIME ft;
	LARGE_INTEGER li;
	__int64 t;
	static int tzflag;
	const __int64 EPOCHFILETIME = 116444736000000000i64;

	GetSystemTimeAsFileTime(&ft);
	li.LowPart  = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	t  = li.QuadPart;       /* In 100-nanosecond intervals */
	t -= EPOCHFILETIME;     /* Offset to the Epoch time */
	t /= 10;                /* In microseconds */
	tv.tv_sec  = (long)(t / 1000000);
	tv.tv_usec = (long)(t % 1000000);
#else
	gettimeofday(&tv, 0);
#endif
	return tv;
}

/* routine used to shutdown and close nicely.*/
void hangup_call()
{
	mylog("Dump call\n");
	iaxc_dump_call();
	mylog("Sleep for 500 msec\n");
	iaxc_millisleep(500);
	mylog("Stop processing thread\n");
	iaxc_stop_processing_thread();
	mylog("Calling iaxc_shutdown...\n");
	iaxc_shutdown();
}

void signal_handler(int signum)
{
	if ( signum == SIGTERM || signum == SIGINT )
	{
		running = 0;
	}
}

void fatal_error(char *err)
{
	mylog("FATAL ERROR: %s\n", err);
}

int levels_callback(float input, float output) {
	//mylog("Input level: %f\nOutput level: %f\n", input, output);
	return 1;
}

int netstat_callback(struct iaxc_ev_netstats n) {
	static int i;

	if ( !print_netstats )
		return 0;

	if ( i++ % 25 == 0 )
		mylog("RTT\t"
		"Rjit\tRlos%%\tRlosC\tRpkts\tRdel\tRdrop\tRooo\t"
		"Ljit\tLlos%%\tLlosC\tLpkts\tLdel\tLdrop\tLooo\n");

	mylog("%d\t"
		"%d\t%d\t%d\t%d\t%d\t%d\t%d\t"
		"%d\t%d\t%d\t%d\t%d\t%d\t%d\n",

		n.rtt,

		n.remote.jitter,
		n.remote.losspct,
		n.remote.losscnt,
		n.remote.packets,
		n.remote.delay,
		n.remote.dropped,
		n.remote.ooo,

		n.local.jitter,
		n.local.losspct,
		n.local.losscnt,
		n.local.packets,
		n.local.delay,
		n.local.dropped,
		n.local.ooo
		);

	return 0;
}

void process_text_message(const char * message)
{
	unsigned int prefs;
	const char ctrl_str[] = "CONTROL:";
	const int ctrl_strlen = strlen(ctrl_str);

	if ( strncmp(message, ctrl_str, ctrl_strlen) == 0 )
	{
		message += ctrl_strlen;
		if ( strcmp(message, "STOPVIDEO") == 0 )
		{
			// Stop sending video
			prefs = iaxc_get_video_prefs();
			prefs = prefs | IAXC_VIDEO_PREF_SEND_DISABLE ;
			iaxc_set_video_prefs(prefs);
		}
		else if ( strcmp(message, "STARTVIDEO") == 0 )
		{
			// Start sending video
			prefs = iaxc_get_video_prefs();
			prefs = prefs & ~IAXC_VIDEO_PREF_SEND_DISABLE ;
			iaxc_set_video_prefs(prefs);
		}
	}
	else
		mylog("Text message received: %s\n", message);
}

void usage()
{
	printf("Usage: stresstest <options> <dial_string>\n\n"
		"available options:\n"
		"  -F <codec> <framerate> <bitrate> <width> <height> <fragsize> set video parameters\n"
		"  -o <filename> media file to run\n"
		"  -a start with audio muted\n"
		"  -v start with video 'muted'\n"
		"  -l run file in a loop\n"
		"  -n dump periodic netstats to log file\n"
		"  -t <TIMEOUT> terminate call after TIMEOUT seconds\n"
		"  -c <TIMEOUT> try connecting for TIMEOUT seconds (default 5)\n"
		"  -L <FILE> log to FILE\n"
		"\n"
		);
	exit(1);
}

int test_mode_state_callback(struct iaxc_ev_call_state s)
{
	mylog("Call #%d state %d\n", s.callNo, s.state);

	if ( s.state & IAXC_CALL_STATE_COMPLETE )
	{
		mylog("Call answered\n");
		call_established = 1;
	}
	if (s.state == IAXC_CALL_STATE_FREE)
	{
		mylog("Call terminated\n");
		running = 0;
	}

	return 0;
}

int test_mode_callback(iaxc_event e)
{
	switch ( e.type )
	{
	case IAXC_EVENT_LEVELS:
		return levels_callback(e.ev.levels.input, e.ev.levels.output);
	case IAXC_EVENT_NETSTAT:
		return netstat_callback(e.ev.netstats);
	case IAXC_EVENT_TEXT:
		process_text_message(e.ev.text.message);
		break;
	case IAXC_EVENT_STATE:
		return test_mode_state_callback(e.ev.call);
	case IAXC_EVENT_VIDEO:
		video_frames_count++;
		break;
	case IAXC_EVENT_AUDIO:
		audio_frames_count++;
		break;
	default:
		break;
	}

	return 0;
}

long msecdiff(struct timeval *t0, struct timeval *t1)
{
	return (t1->tv_sec - t0->tv_sec) * 1000L + (t1->tv_usec - t0->tv_usec) / 1000L;
}

unsigned int STDCALL check_for_command(void *param)
{
	int *cmd_thread_ready = (int *)param;

	*cmd_thread_ready = 1;

	while ( 1 )
	{
		char data[512];
		char *str;

		if ( fgets(data, sizeof(data), stdin) )
		{

			data[sizeof(data) - 1] = 0;

			if ( !(str = strtok(data, "\n")) )
			{
				mylog("WARNING: invalid input\n");
			}
			else if ( !strncmp(str, STRESS_CMD_UNMUTE_AUDIO,
					strlen(STRESS_CMD_UNMUTE_AUDIO)) )
			{
				// mylog("got command to unmute audio\n");
				send_audio = 1;
			}
			else if ( !strncmp(str, STRESS_CMD_MUTE_AUDIO,
					strlen(STRESS_CMD_MUTE_AUDIO)) )
			{
				// mylog("got command to mute audio\n");
				send_audio = 0;
			}
			else if ( !strncmp(str, STRESS_CMD_UNMUTE_VIDEO,
					strlen(STRESS_CMD_UNMUTE_VIDEO)) )
			{
				// mylog("got command to unmute video\n");
				send_video = 1;
			}
			else if ( !strncmp(str, STRESS_CMD_MUTE_VIDEO,
					strlen(STRESS_CMD_MUTE_VIDEO)) )
			{
				// mylog("got command to mute video\n");
				send_video = 0;
			}
			else if ( !strncmp(str, STRESS_CMD_SHOW_STATE,
					strlen(STRESS_CMD_SHOW_STATE)) )
			{
				mylog("got command to show state:\n");
				mylog("running = %d    call_established = %d\n",
						running, call_established);
				mylog("send_video = %d   send audio = %d\n",
						send_video, send_audio);
				mylog("audio frames recvd = %d    video frames recvd = %d\n",
						audio_frames_count, video_frames_count);
				mylog("audio frames sent = %d\n\n", audio_sent_count);
			}
			else if ( !strncmp(str, STRESS_CMD_QUIT,
					strlen(STRESS_CMD_QUIT)) )
			{
				mylog("got command to quit\n");
				running = 0;
			}
			else
			{
				mylog("invalid input: '%s'\n", str);
			}
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	int i;
	char *dest = NULL;
	char *ogg_file = NULL;
	int loop = 0;
	a_thread command_thread = 0;
	unsigned int command_thread_id;
	int ret = -1;
	int cmd_thread_ready = 0;

	/* install signal handler to catch CRTL-Cs */
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	/* Parse command line */
	for ( i = 1; i < argc; i++)
	{
		if ( argv[i][0] == '-' )
		{
			switch ( argv[i][1] )
			{
			case 'F': /* set video params */
				if ( i+6 >= argc )
					usage();
				formatp = 1 << atoi(argv[++i]);
				framerate = atoi(argv[++i]);
				bitrate = atoi(argv[++i]);
				width = atoi(argv[++i]);
				height = atoi(argv[++i]);
				fragsize = atoi(argv[++i]);
				break;
			case 'o':
				if ( i+1 >= argc )
					usage();
				ogg_file = argv[++i];
				break;
			case 'v':
				send_video = 0;
				break;
			case 'a':
				send_audio = 0;
				break;
			case 'l':
				loop = 1;
				break;
			case 'n':
				print_netstats = 1;
				break;
			case 't':
				if ( i+1 >= argc )
					usage();
				call_timeout_ms = 1000 * atoi(argv[++i]);
				break;
			case 'c':
				if ( i+1 >= argc )
					usage();
				connect_timeout_ms = 1000 * atoi(argv[++i]);
				break;
			case 'L':
				if ( i+1 >= argc )
					usage();
				if ( !(log_file = fopen(argv[++i], "w")) )
				{
					mylog("failed to open log \"%s\"\n",
							argv[i]);
					exit(1);
				}
				break;
			default:
				usage();
			}
		}
		else
		{
			dest = argv[i];
		}
	}

	if ( dest == NULL )
	{
		mylog("No destination, quitting\n");
		goto exit;
	}

	if ( ogg_file )
	{
		if ( load_ogg_file(ogg_file) )
		{
			mylog("Failed loading ogg file. Quitting.\n");
			goto exit;
		}
	}
	else
		mylog("No media file, running dry\n");

	// Get start time for timeouts
	start_time = get_now();

	// Initialize iaxclient
	iaxc_video_format_set(formatp, format, framerate, bitrate,
			width, height, fragsize);
	iaxc_set_test_mode(1);

	if (iaxc_initialize(1))
	{
		fatal_error("cannot initialize iaxclient!");
		ret = TEST_UNKNOWN_ERROR;
		goto exit;
	}

	iaxc_set_formats(IAXC_FORMAT_SPEEX, IAXC_FORMAT_SPEEX);
	iaxc_video_bypass_jitter(0);
	iaxc_set_audio_prefs(IAXC_AUDIO_PREF_RECV_REMOTE_ENCODED);
	iaxc_set_video_prefs(IAXC_VIDEO_PREF_RECV_REMOTE_ENCODED);
	iaxc_set_event_callback(test_mode_callback);

	// Crank the engine
	if ( iaxc_start_processing_thread() < 0 )
	{
		fatal_error("failed iaxc_start_processing_thread()\n");
		ret = TEST_UNKNOWN_ERROR;
		goto exit;
	}

	// spin a thread to check for commands from stdin
	if ( ret = create_thread(&command_thread,
				&check_for_command,
				(void *)&cmd_thread_ready, &command_thread_id) )
	{
		mylog("failed creating command thread\n");
		goto exit;
	}
	else
	{
		while ( !cmd_thread_ready )
			iaxc_millisleep(10);
	}

	// Dial out
	if ( iaxc_call(dest) < 0 )
	{
		fatal_error("failed iaxc_call()");
		ret = TEST_UNKNOWN_ERROR;
		goto exit;
	}

	// Wait for the call to be established;
	while ( !call_established && running )
	{
		struct timeval now = get_now();
		if ( connect_timeout_ms > 0 &&
				msecdiff(&start_time, &now) > connect_timeout_ms )
		{
			hangup_call();
			ret = TEST_NO_CONNECTION;
			goto exit;
		}
		iaxc_millisleep(5);
	}

	while ( running )
	{
		struct timeval now = get_now();

		// We only need this if we actually want to send something
		if ( ogg_file )
		{
			ogg_packet *op;

			op = get_next_audio_op(now);
			if ( !loop && audio_is_eos() )
				break;
			if ( send_audio && op != NULL && op->bytes > 0 )
			{
				iaxc_push_audio(op->packet, op->bytes,
						SPEEX_SAMPLING_RATE *
						SPEEX_FRAME_DURATION / 1000);
				audio_sent_count++;
			}

			op = get_next_video_op(now);
			if ( !loop && video_is_eos() )
				break;
			if ( send_video && op != NULL && op->bytes > 0 )
			{
				if ( !camera_enabled_sent )
				{
					/* Let app_conference know that we can send video */
					iaxc_send_text("CONTROL:CAMERA_ENABLED");
					camera_enabled_sent = 1;
				}
				iaxc_push_video(op->packet, op->bytes, 1);
			}
		}

		// Tight spinloops are bad, mmmkay?
		iaxc_millisleep(5);

		// Exit after a positive timeout
		if ( call_timeout_ms > 0 &&
				msecdiff(&start_time, &now) > call_timeout_ms )
			running = 0;
	}

	mylog("Received %d audio frames and %d video frames\n",
			audio_frames_count, video_frames_count);

	if ( audio_frames_count == 0 && video_frames_count == 0 )
		ret = TEST_NO_MEDIA;
	else
		ret = TEST_OK;

	hangup_call();

exit:
	mylog("Exiting with code %d\n", ret);

	return ret;
}
