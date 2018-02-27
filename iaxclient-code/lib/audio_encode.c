/*
 * iaxclient: a cross-platform IAX softphone library
 *
 * Copyrights:
 * Copyright (C) 2003-2006, Horizon Wimba, Inc.
 * Copyright (C) 2007, Wimba, Inc.
 *
 * Contributors:
 * Steve Kann <stevek@stevek.com>
 * Michael Van Donselaar <mvand@vandonselaar.org>
 * Shawn Lawrence <shawn.lawrence@terracecomm.com>
 *
 * This program is free software, distributed under the terms of
 * the GNU Lesser (Library) General Public License.
 */

#include "audio_encode.h"
#include "iaxclient_lib.h"
#include "iax-client.h"
#ifdef CODEC_GSM
#include "codec_gsm.h"
#endif
#include "codec_ulaw.h"
#include "codec_alaw.h"
#include "codec_speex.h"
#include <speex/speex_preprocess.h>

/* Determine if we should do AEC */
#if defined(SPEEX_EC) && !defined(WIN32)
#define DO_EC
#else
#undef DO_EC
#endif

#ifdef DO_EC
#include <speex/speex_echo.h>
#include "ringbuffer.h"
#endif

#ifdef CODEC_ILBC
#include "codec_ilbc.h"
#endif

static float iaxci_silence_threshold = AUDIO_ENCODE_SILENCE_DB;

static float input_level = 0.0f;
static float output_level = 0.0f;

int iaxci_sample_rate = 8000;

static SpeexPreprocessState *st = NULL;
static int speex_state_size = 0;
static int speex_state_rate = 0;
static int iaxci_filters =
		IAXC_FILTER_AGC |
		IAXC_FILTER_DENOISE |
		IAXC_FILTER_AAGC |
		IAXC_FILTER_CN;

static MUTEX audio_lock;

/* echo_tail length, in samples */
#define ECHO_TAIL 512

/* Maximum attenuation of residual echo in dB (negative number) */
#define ECHO_SUPPRESS -60
/* Maximum attenuation of residual echo when near end is active, in dB (negative number) */
#define ECHO_SUPPRESS_ACTIVE -60

/* Size of ring buffer used for echo cancellation. Must be power of 2. */
#define EC_RING_SIZE  512

#ifdef DO_EC
static SpeexEchoState *ec = 0;
static rb_RingBuffer ecOutRing;
static char outRingBuf[EC_RING_SIZE];
#endif

/* use to measure time since last audio was processed */
static struct timeval timeLastInput ;
static struct timeval timeLastOutput ;

static struct iaxc_speex_settings speex_settings =
{
	1,    /* decode_enhance */
	-1,   /* float quality */
	-1,   /* bitrate */
	0,    /* vbr */
	0,    /* abr */
	3     /* complexity */
};


static float vol_to_db(float vol)
{
	/* avoid calling log10() on zero which yields inf or
	 * negative numbers which yield nan */
	if ( vol <= 0.0f )
		return AUDIO_ENCODE_SILENCE_DB;
	else
		return log10f(vol) * 20.0f;
}

static int do_level_callback()
{
	static struct timeval last = {0,0};
	struct timeval now;
	float input_db;
	float output_db;

	now = iax_tvnow();

	if ( last.tv_sec != 0 && iaxci_usecdiff(&now, &last) < 100000 )
		return 0;

	last = now;

	/* if input has not been processed in the last second, set to silent */
	input_db = iaxci_usecdiff(&now, &timeLastInput) < 1000000 ?
			vol_to_db(input_level) : AUDIO_ENCODE_SILENCE_DB;

	/* if output has not been processed in the last second, set to silent */
	output_db = iaxci_usecdiff(&now, &timeLastOutput) < 1000000 ?
		vol_to_db(output_level) : AUDIO_ENCODE_SILENCE_DB;

	iaxci_do_levels_callback(input_db, output_db);

	return 0;
}

static void set_speex_filters()
{
	int i;

	if ( !st )
		return;

	i = 1; /* always make VAD decision */
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_VAD, &i);
	i = (iaxci_filters & IAXC_FILTER_AGC) ? 1 : 0;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &i);
	i = (iaxci_filters & IAXC_FILTER_DENOISE) ? 1 : 0;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &i);
	i = (iaxci_filters & IAXC_FILTER_DEREVERB) ? 1 : 0;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &i);

	/*
	 * We can tweak these parameters to play with VAD sensitivity.
	 * For now, we use the default values since it seems they are a good starting point.
	 * However, if need be, this is the code that needs to change
	 */
	i = 35;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_START, &i);
	i = 20;
	speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &i);
}

static void calculate_level(const short *audio, int len, float *level)
{
	int big_sample = 0;
	int i;

	for ( i = 0; i < len; i++ )
	{
		const int sample = abs(audio[i]);
		big_sample = sample > big_sample ?
			sample : big_sample;
	}

	*level += ((float)big_sample / 32767.0f - *level) / 5.0f;
}

static int input_postprocess(short * audio, int len, int rate)
{
	static int aagc_frame_count = 0;
	static int aagc_periods_to_skip = 0;

	const int using_vad = iaxci_silence_threshold > 0.0f;
	const int aagc_period = rate / len; /* 1 second */

	int speaking = 1;
	int loudness = 0;

	MUTEXLOCK(&audio_lock);

	if ( !st || speex_state_size != len || speex_state_rate != rate )
	{
		if (st)
			speex_preprocess_state_destroy(st);
		st = speex_preprocess_state_init(len,rate);
#ifdef DO_EC
		if ( ec )
		{
			int i;

			speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_ECHO_STATE, ec);
			i = ECHO_SUPPRESS;
			speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS, &i);
			i = ECHO_SUPPRESS_ACTIVE;
			speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS_ACTIVE, &i);
		}
#endif
		speex_state_size = len;
		speex_state_rate = rate;
		set_speex_filters();
	}

	/* go through the motions only if we need at least one of the
	 * preprocessor filters */
	if ( using_vad || (iaxci_filters &
				(IAXC_FILTER_DENOISE |
				 IAXC_FILTER_AGC |
				 IAXC_FILTER_DEREVERB |
				 IAXC_FILTER_ECHO)) )
	{
		speaking = speex_preprocess_run(st, audio);
		speex_preprocess_ctl(st, SPEEX_PREPROCESS_GET_AGC_LOUDNESS,
				&loudness);
	}

	MUTEXUNLOCK(&audio_lock);

	/* If we are using the VAD test and if speex indicates non-speaking,
	 * ignore the computed input level and indicate to the user that the
	 * input level was zero.
	 */
	if ( using_vad && !speaking )
		input_level = 0.0f;
	else
		calculate_level(audio, len, &input_level);

	/* Analog Automatic Gain Control, AAGC. */
	if ( speaking && iaxci_silence_threshold != 0.0f &&
			(iaxci_filters & IAXC_FILTER_AGC) &&
			(iaxci_filters & IAXC_FILTER_AAGC) &&
			++aagc_frame_count % aagc_period == 0 &&
			!aagc_periods_to_skip-- )
	{
		/* This heuristic uses the loudness value from the speex
		 * preprocessor to determine a new mixer level. The loudness
		 * ranges from 0 to up over 80. When mixer level, speex AGC,
		 * and the actual speaker's level are in equilibrium, the
		 * loudness tends to be from 4 to 16. When the loudness goes
		 * above this comfortable range, there is a risk of the input
		 * signal being clipped. AAGC's primary purpose is to avoid
		 * clipping.
		 *
		 * After a loud event (think cough), the loudness level will
		 * spike and then decay over time (assuming the speaker
		 * speaking at a relatively constant level). To avoid
		 * over-adjusting, we skip some number of aagc sampling periods
		 * before making any more adjustments.  This gives the loudness
		 * value time to normalize after one-time spikes in the input
		 * level.
		 */

		/* The mixer level is a percentage ranging from 0.00 to 1.00 */
		const float mixer_level = iaxc_input_level_get();
		float new_mixer_level = mixer_level;

		if ( loudness > 40 )
		{
			new_mixer_level -= 0.20f;
			aagc_periods_to_skip = 8;
		}
		else if ( loudness > 25 )
		{
			new_mixer_level -= 0.15f;
			aagc_periods_to_skip = 4;
		}
		else if ( loudness > 15 )
		{
			new_mixer_level -= 0.10f;
			aagc_periods_to_skip = 2;
		}
		else if ( loudness > 12 )
		{
			new_mixer_level -= 0.05f;
			aagc_periods_to_skip = 4;
		}
		else if ( loudness < 2 )
		{
			new_mixer_level += 0.15f;
			aagc_periods_to_skip = 4;
		}
		else if ( loudness < 4 )
		{
			new_mixer_level += 0.10f;
			aagc_periods_to_skip = 4;
		}
		else
		{
			aagc_periods_to_skip = 0;
		}

		/* Normalize the proposed new mixer level */
		if ( new_mixer_level < 0.05f )
			new_mixer_level = 0.05f;
		else if ( new_mixer_level > 1.00f )
			new_mixer_level = 1.00f;

		if ( new_mixer_level != mixer_level )
			iaxc_input_level_set(new_mixer_level);
	}

	do_level_callback();

	return using_vad ? !speaking :
		vol_to_db(input_level) < iaxci_silence_threshold;
}

static int output_postprocess(const short * audio, int len)
{
	calculate_level(audio, len, &output_level);

	do_level_callback();

	return 0;
}

static struct iaxc_audio_codec *create_codec(int format)
{
	switch (format & IAXC_AUDIO_FORMAT_MASK)
	{
#ifdef CODEC_GSM
	case IAXC_FORMAT_GSM:
		return codec_audio_gsm_new();
#endif
	case IAXC_FORMAT_ULAW:
		return codec_audio_ulaw_new();
	case IAXC_FORMAT_ALAW:
		return codec_audio_alaw_new();
	case IAXC_FORMAT_SPEEX:
		return codec_audio_speex_new(&speex_settings);
#ifdef CODEC_ILBC
	case IAXC_FORMAT_ILBC:
		return codec_audio_ilbc_new();
#endif
	default:
		/* ERROR: codec not supported */
		fprintf(stderr, "ERROR: Codec not supported: %d\n", format);
		return NULL;
	}
}

EXPORT void iaxc_set_speex_settings(int decode_enhance, float quality,
		int bitrate, int vbr, int abr, int complexity)
{
	speex_settings.decode_enhance = decode_enhance;
	speex_settings.quality = quality;
	speex_settings.bitrate = bitrate;
	speex_settings.vbr = vbr;
	speex_settings.abr = abr;
	speex_settings.complexity = complexity;
}

int audio_send_encoded_audio(struct iaxc_call *call, int callNo, void *data,
		int format, int samples)
{
	unsigned char outbuf[1024];
	int outsize = 1024;
	int silent;
	int insize = samples;

	/* update last input timestamp */
	timeLastInput = iax_tvnow();

	silent = input_postprocess(data, insize, 8000);

	if(silent)
	{
		if(!call->tx_silent)
		{  /* send a Comfort Noise Frame */
			call->tx_silent = 1;
			if ( iaxci_filters & IAXC_FILTER_CN )
				iax_send_cng(call->session, 10, NULL, 0);
		}
		return 0;  /* poof! no encoding! */
	}

	/* we're going to send voice now */
	call->tx_silent = 0;

	/* destroy encoder if it is incorrect type */
	if(call->encoder && call->encoder->format != format)
	{
		call->encoder->destroy(call->encoder);
		call->encoder = NULL;
	}

	/* just break early if there's no format defined: this happens for the
	 * first couple of frames of new calls */
	if(format == 0) return 0;

	/* create encoder if necessary */
	if(!call->encoder)
	{
		call->encoder = create_codec(format);
	}

	if(!call->encoder)
	{
		/* ERROR: no codec */
		fprintf(stderr, "ERROR: Codec could not be created: %d\n", format);
		return 0;
	}

	if(call->encoder->encode(call->encoder, &insize, (short *)data,
				&outsize, outbuf))
	{
		/* ERROR: codec error */
		fprintf(stderr, "ERROR: encode error: %d\n", format);
		return 0;
	}

	if(samples-insize == 0)
	{
		fprintf(stderr, "ERROR encoding (no samples output (samples=%d)\n", samples);
		return -1;
	}

	// Send the encoded audio data back to the app if required
	// TODO: fix the stupid way in which the encoded audio size is returned
	if ( iaxc_get_audio_prefs() & IAXC_AUDIO_PREF_RECV_LOCAL_ENCODED )
		iaxci_do_audio_callback(callNo, 0, IAXC_SOURCE_LOCAL, 1,
				call->encoder->format & IAXC_AUDIO_FORMAT_MASK,
				sizeof(outbuf) - outsize, outbuf);

	if(iax_send_voice(call->session,format, outbuf,
				sizeof(outbuf) - outsize, samples-insize) == -1)
	{
		fprintf(stderr, "Failed to send voice! %s\n", iax_errstr);
		return -1;
	}

	return 0;
}

/* decode encoded audio; return the number of bytes decoded
 * negative indicates error */
int audio_decode_audio(struct iaxc_call * call, void * out, void * data,
		int len, int format, int * samples)
{
	int insize = len;
	int outsize = *samples;

	timeLastOutput = iax_tvnow();

	if ( format == 0 )
	{
		fprintf(stderr, "audio_decode_audio: Format is zero "
				"(should not happen)!\n");
		return -1;
	}

	/* destroy decoder if it is incorrect type */
	if ( call->decoder && call->decoder->format != format )
	{
		call->decoder->destroy(call->decoder);
		call->decoder = NULL;
	}

	/* create decoder if necessary */
	if ( !call->decoder )
	{
		call->decoder = create_codec(format);
	}

	if ( !call->decoder )
	{
		fprintf(stderr, "ERROR: Codec could not be created: %d\n",
				format);
		return -1;
	}

	if ( call->decoder->decode(call->decoder,
				&insize, (unsigned char *)data,
				&outsize, (short *)out) )
	{
		fprintf(stderr, "ERROR: decode error: %d\n", format);
		return -1;
	}

	output_postprocess(out, *samples - outsize);

	*samples = outsize;
	return len - insize;
}

EXPORT int iaxc_get_filters(void)
{
	return iaxci_filters;
}

EXPORT void iaxc_set_filters(int filters)
{
	iaxci_filters = filters;
	set_speex_filters();
}

EXPORT void iaxc_set_silence_threshold(float thr)
{
	iaxci_silence_threshold = thr;
	set_speex_filters();
}

int audio_echo_cancellation(short *inputBuffer, short *outputBuffer, int samples)
{
#ifdef DO_EC
	int i;
	short delayedBuf[1024];
	short cancelledBuffer[1024];

	/* if ec is off, clear ec state -- this way, we start fresh if/when
	* it's turned back on. */
	MUTEXLOCK(&audio_lock);
	if ( !(iaxci_filters & IAXC_FILTER_ECHO) )
	{
		if ( ec )
		{
			speex_echo_state_destroy(ec);
			ec = NULL;
			if ( st )
			{
				speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_ECHO_STATE, NULL);
			}
		}

		MUTEXUNLOCK(&audio_lock);
		return 0;
	}

	/* we want echo cancellation */
	if ( !ec )
	{
		rb_InitializeRingBuffer(&ecOutRing, EC_RING_SIZE, &outRingBuf);
		ec = speex_echo_state_init(SAMPLES_PER_FRAME, ECHO_TAIL);

		if ( st )
		{
			speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_ECHO_STATE, ec);
			i = ECHO_SUPPRESS;
			speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS, &i);
			i = ECHO_SUPPRESS_ACTIVE;
			speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_ECHO_SUPPRESS_ACTIVE, &i);
		}
	}
	MUTEXUNLOCK(&audio_lock);

	// Put our data in the EC ring buffer.
	// Echo canceller needs SAMPLES_PER_FRAME samples, so if we don't have enough
	// at this time, we just store what we have and return.
	rb_WriteRingBuffer(&ecOutRing, outputBuffer, samples * 2);
	if ( rb_GetRingBufferReadAvailable(&ecOutRing) < (SAMPLES_PER_FRAME * 2) )
		return -1;

	rb_ReadRingBuffer(&ecOutRing, delayedBuf, SAMPLES_PER_FRAME * 2);

	/* TODO: speex_echo_cancellation() and speex_preprocess_run() operate
	 * on the same state and thus must be serialized. Because the audio
	 * lock is not held, this call has the potential to mess-up the
	 * preprocessor (which is serialized by the audio lock). I believe the
	 * net effect of this problem is to break residual echo cancellation
	 * when these calls overlap. Unfortunately, just serializing this
	 * speex_echo_cancellation() call with the audio lock may not be
	 * sufficient since the next call to speex_preprocess_run() is counting
	 * on operating on this cancelledBuffer -- since we buffer the input
	 * audio (cancelledBuffer), we are actually explicitly decoupling the
	 * calls to speex_echo_cancellation() and speex_preprocess_run(). Oops.
	 *
	 * In other words, it should go like this:
	 *
	 *   speex_echo_cancellation(A)
	 *   speex_preprocess_run(A)
	 *   speex_echo_cancellation(B)
	 *   speex_preprocess_run(B)
	 *   speex_echo_cancellation(C)
	 *   speex_preprocess_run(C)
	 *
	 * but it actually may be going like this:
	 *
	 *   speex_echo_cancellation(A)
	 *   speex_echo_cancellation(B)
	 *   speex_preprocess_run(A) -- bad, residual echo from B is applied to A
	 *   speex_echo_cancellation(C)
	 *   speex_preprocess_run(B) -- bad, residual echo from C is applied to B
	 *   speex_preprocess_run(C)
	 */
	speex_echo_cancellation(ec, inputBuffer, delayedBuf, cancelledBuffer);

	memcpy(inputBuffer, cancelledBuffer, samples * sizeof(short));
#endif
	return 0;
}

int audio_initialize()
{
	MUTEXINIT(&audio_lock);
	return 0;
}

int audio_destroy()
{
	MUTEXLOCK(&audio_lock);
	if ( st )
	{
		speex_preprocess_state_destroy(st);
		st = NULL;
	}
#ifdef DO_EC
	if ( ec )
	{
		speex_echo_state_destroy(ec);
		ec = NULL;
	}
#endif
	MUTEXUNLOCK(&audio_lock);
	MUTEXDESTROY(&audio_lock);
	return 0;
}
