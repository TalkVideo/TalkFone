/*
 * iaxclient: a cross-platform IAX softphone library
 *
 * Copyrights:
 * Copyright (C) 2003-2006, Horizon Wimba, Inc.
 * Copyright (C) 2007, Wimba, Inc.
 *
 * Contributors:
 * Steve Kann <stevek@stevek.com>
 *
 * This program is free software, distributed under the terms of
 * the GNU Lesser (Library) General Public License.
 */

#ifndef _AUDIO_ENCODE_H
#define _AUDIO_ENCODE_H

/* Some audio parameters */
#define MAX_SAMPLE_RATE       48000
#ifndef MS_PER_FRAME
# define MS_PER_FRAME         20
#endif
#define SAMPLES_PER_FRAME     (MS_PER_FRAME * iaxci_sample_rate / 1000)
#define MAX_SAMPLES_PER_FRAME (MS_PER_FRAME * MAX_SAMPLE_RATE  / 1000)

extern int iaxci_sample_rate;

/* Minimum dB possible in the iaxclient world. This level
 * is intended to represent silence.
 */
#define AUDIO_ENCODE_SILENCE_DB -99.0f

struct iaxc_call;
struct iax_event;

int audio_initialize();
int audio_destroy();

int audio_send_encoded_audio(struct iaxc_call * most_recent_answer, int callNo,
		void * data, int iEncodeType, int samples);

int audio_decode_audio(struct iaxc_call * p, void * out, void * data, int len,
		int iEncodeType, int * samples);

int audio_echo_cancellation(short *inputBuffer, short *outputBuffer, int samples);

#endif

