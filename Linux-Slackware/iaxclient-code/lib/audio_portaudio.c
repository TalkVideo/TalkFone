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
 * Erik Bunce <kde@bunce.us>
 *
 * This program is free software, distributed under the terms of
 * the GNU Lesser (Library) General Public License.
 *
 * Module: audio_portaudio
 * Purpose: Audio code to provide portaudio driver support for IAX library
 * Developed by: Shawn Lawrence, Terrace Communications Inc.
 * Creation Date: April 18, 2003
 *
 * This library uses the PortAudio Portable Audio Library
 * For more information see: http://www.portaudio.com/
 * PortAudio Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 */

#if defined(WIN32)  ||  defined(_WIN32_WCE)
#include <stdlib.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#include "audio_portaudio.h"
#include "audio_encode.h"
#include "iaxclient_lib.h"
#include "ringbuffer.h"
#include "portmixer.h"

typedef short SAMPLE;

static PaStream *iStream, *oStream, *aStream;
static PxMixer *iMixer = NULL, *oMixer = NULL;

static int selectedInput, selectedOutput, selectedRing;

static int mixers_initialized;

/* RingBuffer Size; Needs to be Pow(2), 1024 = 512 samples = 64ms */
#ifndef OUTRBSZ
# define OUTRBSZ 32768
#endif

/* Input ringbuffer size;  this doesn't seem to be as critical, and making it big
 * causes issues when we're answering calls, etc., and the audio system is running
 * but not being drained */
#ifndef INRBSZ
# define INRBSZ  2048
#endif

/* TUNING:  The following constants may help in tuning for situations
 * where you are getting audio-level under/overruns.
 *
 * If you are running iaxclient on a system where you cannot get
 * low-latency scheduling, you may need to increase these.  This tends
 * to be an issue on non-MacOSX unix systems, when you are not running
 * as root, and cannot ask the OS for higher priority.
 *
 * RBOUTTARGET:  This a target size of the output ringbuffer, in milliseconds,
 * where audio for your speakers goes after being decoded and mixed, and
 * before the audio callback asks for it.  It can get larger than this
 * (up to OUTRBSZ, above), but when it does, for a bit, we will start
 * dropping some frames.  For no drops at all, this needs to be set to
 * contain the number of samples in your largest scheduling gap
 *
 * PA_NUMBUFFERS:  This is the number of buffers that the low-level
 * operating system driver will use, for buffering our output (and also
 * our input) between the soundcard and portaudio.  This should also be
 * set to the maximum scheduling delay.  Some drivers, though, will
 * callback _into_ portaudio with a higher priority, so this doesn't
 * necessarily need to be as big as RBOUTMAXSZ, although on linux, it
 * does.  The default is to leave this up to portaudio..
 */

/* 80ms if average outRing length is more than this many bytes, start dropping */
#ifndef RBOUTTARGET
# define RBOUTTARGET (80)
#endif

/* size in bytes of ringbuffer target */
#define RBOUTTARGET_BYTES (RBOUTTARGET * (iaxci_sample_rate / 1000) * sizeof(SAMPLE))

static char inRingBuf[INRBSZ], outRingBuf[OUTRBSZ];
static rb_RingBuffer inRing, outRing;

/* TODO: This is used without explicit initialization */
static int outRingLenAvg;

static int oneStream;
static int auxStream;
static int virtualMonoIn;
static int virtualMonoOut;
static int virtualMonoRing;

static int running;

static struct iaxc_sound *sounds;
static int  nextSoundId = 1;

static MUTEX sound_lock;

/* forward declarations */
static int pa_start (struct iaxc_audio_driver *d );
static void handle_paerror(PaError err, char * where);
static int pa_input_level_set(struct iaxc_audio_driver *d, float level);
static float pa_input_level_get(struct iaxc_audio_driver *d);

/* scan devices and stash pointers to dev structures.
 *  But, these structures only remain valid while Pa is initialized,
 *  which, with pablio, is only while it's running!
 *  Also, storing these things in two separate arrays loses the actual
 *  PaDeviceID's associated with devices (since their index in these
 *  input/output arrays isn't the same as their index in the combined
 *  array */
static int scan_devices(struct iaxc_audio_driver *d)
{
	int nDevices;
	int i;

	d->nDevices = nDevices = Pa_GetDeviceCount();
	d->devices = (struct iaxc_audio_device *)
		malloc(nDevices * sizeof(struct iaxc_audio_device));

	for ( i=0; i < nDevices; i++ )
	{
		const PaDeviceInfo *pa;
		struct iaxc_audio_device *dev;

		pa=Pa_GetDeviceInfo(i);
		dev = &(d->devices[i]);

		if ( pa ) //frik: under Terminal Services this is NULL
		{
			dev->name = (char *)pa->name;
			dev->devID = i;
			dev->capabilities = 0;

			if ( pa->maxInputChannels > 0 )
				dev->capabilities |= IAXC_AD_INPUT;

			if ( pa->maxOutputChannels > 0 )
			{
				dev->capabilities |= IAXC_AD_OUTPUT;
				dev->capabilities |= IAXC_AD_RING;
			}

			if ( i == Pa_GetDefaultInputDevice() )
				dev->capabilities |= IAXC_AD_INPUT_DEFAULT;

			if ( i == Pa_GetDefaultOutputDevice() )
			{
				dev->capabilities |= IAXC_AD_OUTPUT_DEFAULT;
				dev->capabilities |= IAXC_AD_RING_DEFAULT;
			}
		}
		else //frik: under Terminal Services
		{
			dev->name = "Not usable device";
			dev->devID = i;
			dev->capabilities = 0;
		}
	}

	return 0;
}

static void mono2stereo(SAMPLE *out, const SAMPLE *in, int nSamples)
{
	int i;
	for ( i=0; i < nSamples; i++ )
	{
		*(out++) = *in;
		*(out++) = *(in++);
	}
}

static void stereo2mono(SAMPLE *out, const SAMPLE *in, int nSamples)
{
	int i;
	for ( i=0; i < nSamples; i++ )
	{
		*(out) = *(in++);
		out++;
		in++;
	}
}

static void mix_slin(short *dst, short *src, int samples, int virtualMono)
{
	int i=0,val=0;
	for ( i=0; i < samples; i++ )
	{
		if ( virtualMono )
			val = ((short *)dst)[2*i] + ((short *)src)[i];
		else
			val = ((short *)dst)[i] + ((short *)src)[i];

		if ( val > 0x7fff )
		{
			val = 0x7fff-1;
		} else if (val < -0x7fff)
		{
			val = -0x7fff+1;
		}

		if ( virtualMono )
		{
			dst[2*i] = val;
			dst[2*i+1] = val;
		} else
		{
			dst[i] = val;
		}

	}
}

static int pa_mix_sounds (void *outputBuffer, unsigned long frames,
		int channel, int virtualMono)
{
	struct iaxc_sound *s;
	struct iaxc_sound **sp;
	unsigned long outpos;

	MUTEXLOCK(&sound_lock);
	/* mix each sound into the outputBuffer */
	sp = &sounds;
	while ( sp && *sp )
	{
		s = *sp;
		outpos = 0;

		if ( s->channel == channel )
		{
			/* loop over the sound until we've played it enough
			 * times, or we've filled the outputBuffer */
			for(;;)
			{
				int n;

				if ( outpos == frames )
					break;  /* we've filled the buffer */
				if ( s->pos == s->len )
				{
					if ( s->repeat == 0 )
					{
						// XXX free the sound
						// structure, and maybe the
						// buffer!
						(*sp) = s->next;
						if(s->malloced)
							free(s->data);
						free(s);
						break;
					}
					s->pos = 0;
					s->repeat--;
				}

				/* how many frames do we add in this loop? */
				n = (frames - outpos) < (unsigned long)(s->len - s->pos) ?
					(frames - outpos) :
					(unsigned long)(s->len - s->pos);

				/* mix in the frames */
				mix_slin((short *)outputBuffer + outpos,
						s->data+s->pos, n, virtualMono);

				s->pos += n;
				outpos += n;
			}
		}
		if ( *sp ) /* don't advance if we removed this member */
			sp = &((*sp)->next);
	}
	MUTEXUNLOCK(&sound_lock);
	return 0;
}

static int pa_play_sound(struct iaxc_sound *inSound, int ring)
{
	struct iaxc_sound *sound;

	sound = (struct iaxc_sound *)malloc(sizeof(struct iaxc_sound));
	if ( !sound )
		return 1;

	*sound = *inSound;

	MUTEXLOCK(&sound_lock);
	sound->channel = ring;
	sound->id = nextSoundId++;
	sound->pos = 0;

	sound->next = sounds;
	sounds = sound;
	MUTEXUNLOCK(&sound_lock);

	if ( !running )
		pa_start(NULL); /* XXX fixme: start/stop semantics */

	return sound->id;
}

static int pa_stop_sound(int soundID)
{
	struct iaxc_sound **sp;
	int retval = 1; /* not found */

	MUTEXLOCK(&sound_lock);
	for ( sp = &sounds; *sp; (*sp) = (*sp)->next )
	{
		struct iaxc_sound *s = *sp;
		if ( s->id == soundID )
		{
			if ( s->malloced )
				free(s->data);
			/* remove from list */
			(*sp) = s->next;
			free(s);

			retval= 0; /* found */
			break;
		}
	}
	MUTEXUNLOCK(&sound_lock);

	return retval; /* found? */
}

static int pa_callback(const void *inputBuffer, void *outputBuffer,
	    unsigned long samplesPerFrame,
	    const PaStreamCallbackTimeInfo* outTime,
	    PaStreamCallbackFlags statusFlags,
	    void *userData)
{
	short virtualInBuffer[MAX_SAMPLES_PER_FRAME];
	short virtualOutBuffer[MAX_SAMPLES_PER_FRAME];
	int totBytes = samplesPerFrame * sizeof(SAMPLE);

#if 0
	/* I think this can't happen */
	if(virtualMono && samplesPerFrame > SAMPLES_PER_FRAME) {
		fprintf(stderr, "ERROR: buffer in callback is too big!\n");
		exit(1);
	}
#endif
	if ( statusFlags & paOutputUnderflow )
		fprintf(stderr, "WARNING: Output Underflow detected\n");
	if ( statusFlags & paInputUnderflow )
		fprintf(stderr, "WARNING: Input Underflow detected\n");
	if ( statusFlags & paOutputOverflow )
		fprintf(stderr, "WARNING: Output Overflow detected\n");
	if ( statusFlags & paInputOverflow )
		fprintf(stderr, "WARNING: Input Overflow detected\n");

	if ( outputBuffer )
	{
		int bWritten;
		/* output underflow might happen here */
		if ( virtualMonoOut )
		{
			bWritten = rb_ReadRingBuffer(&outRing, virtualOutBuffer,
					totBytes);

			/* we zero "virtualOutBuffer", then convert the whole
			 * thing, yes, because we use virtualOutBuffer for ec
			 * below */
			if ( bWritten < totBytes )
			{
				memset((char *)virtualOutBuffer + bWritten, 0,
						totBytes - bWritten);
				//fprintf(stderr, "*U*");
			}

			mono2stereo((SAMPLE *)outputBuffer, virtualOutBuffer,
					samplesPerFrame);
		}
		else
		{
			bWritten = rb_ReadRingBuffer(&outRing, outputBuffer,
					totBytes);

			if ( bWritten < totBytes )
			{
				memset((char *)outputBuffer + bWritten,
						0, totBytes - bWritten);
				//fprintf(stderr, "*U*");
			}
		}

		/* zero underflowed space [ silence might be more golden
		 * than garbage? ] */

		pa_mix_sounds(outputBuffer, samplesPerFrame, 0, virtualMonoOut);

		if ( !auxStream )
			pa_mix_sounds(outputBuffer, samplesPerFrame, 1,
					virtualMonoOut);
	}


	if ( inputBuffer )
	{
		int res;

		/* input overflow might happen here */
		if ( virtualMonoIn )
		{
			stereo2mono(virtualInBuffer, (SAMPLE *)inputBuffer,
					samplesPerFrame);
			res = audio_echo_cancellation(virtualInBuffer,
			                              virtualOutBuffer,
			                              samplesPerFrame);
			if ( !res )
				rb_WriteRingBuffer(&inRing, virtualInBuffer, totBytes);
		}
		else
		{
			res = audio_echo_cancellation((short *)inputBuffer,
			                              (short *)outputBuffer,
			                              samplesPerFrame);
			if ( !res)
				rb_WriteRingBuffer(&inRing, inputBuffer, totBytes);
		}
	}

	return 0;
}

static int pa_aux_callback(void *inputBuffer, void *outputBuffer,
	    unsigned long samplesPerFrame,
	    const PaStreamCallbackTimeInfo* outTime,
	    PaStreamCallbackFlags statusFlags,
	    void *userData)
{
	int totBytes = samplesPerFrame * sizeof(SAMPLE) * (virtualMonoRing + 1);

	if ( outputBuffer )
	{
		memset((char *)outputBuffer, 0, totBytes);
		pa_mix_sounds(outputBuffer, samplesPerFrame, 1, virtualMonoRing);
	}
	return 0;
}

// NOTE: pa_open does not require an input device for success
static int pa_open(int single, int inMono, int outMono)
{
	PaError err;
	PaDeviceInfo *in_dev_info, *out_dev_info;

	struct PaStreamParameters in_stream_params, out_stream_params, *in_params;

	if ( selectedInput != paNoDevice )
	{
		in_dev_info = (PaDeviceInfo *)Pa_GetDeviceInfo(selectedInput);
		if ( in_dev_info == NULL )
			return -1;

	in_stream_params.device = selectedInput;
	in_stream_params.channelCount = (inMono ? 1 : 2);
	in_stream_params.sampleFormat = paInt16;
		in_stream_params.suggestedLatency = in_dev_info->defaultLowInputLatency;
	in_stream_params.hostApiSpecificStreamInfo = NULL;

		in_params = &in_stream_params;
	}
	else
		in_params = NULL;

	out_dev_info = (PaDeviceInfo *)Pa_GetDeviceInfo(selectedOutput);
	if ( out_dev_info == NULL )
		return -1;

	out_stream_params.device = selectedOutput;
	out_stream_params.channelCount = (outMono ? 1 : 2);
	out_stream_params.sampleFormat = paInt16;
	out_stream_params.suggestedLatency = out_dev_info->defaultLowOutputLatency;
	out_stream_params.hostApiSpecificStreamInfo = NULL;

	// if there is no input device, there's no point in dual streams
	if ( single || selectedInput == paNoDevice )
	{
		err = Pa_OpenStream(&oStream,
			in_params,
			&out_stream_params,
			iaxci_sample_rate,
			SAMPLES_PER_FRAME,
			paNoFlag,
			(PaStreamCallback *)pa_callback,
			NULL);
		if (err != paNoError)
			return -1;

		iStream = oStream;
		oneStream = 1;
	} else
	{
		struct PaStreamParameters no_device;

		no_device.device = paNoDevice;
		no_device.channelCount = 0;
		no_device.sampleFormat = paInt16;
		no_device.suggestedLatency = in_dev_info->defaultLowInputLatency; // FEEDBACK - unsure if appropriate
		no_device.hostApiSpecificStreamInfo = NULL;

		err = Pa_OpenStream(&iStream,
			&in_stream_params,
			&no_device,
			iaxci_sample_rate,
			SAMPLES_PER_FRAME,
			paNoFlag,
			(PaStreamCallback *)pa_callback,
			NULL);
		if ( err != paNoError )
			return -1;

		err = Pa_OpenStream(&oStream,
			&no_device,
			&out_stream_params,
			iaxci_sample_rate,
			SAMPLES_PER_FRAME,
			paNoFlag,
			(PaStreamCallback *)pa_callback,
			NULL);

		if ( err != paNoError )
		{
			Pa_CloseStream(iStream);
			iStream = NULL;
			return -1;
		}
		oneStream = 0;
	}

	virtualMonoIn = (inMono ? 0 : 1);
	virtualMonoOut = (outMono ? 0 : 1);
	return 0;
}

/* some commentary here:
 * 1: MacOSX: MacOSX often needs "virtual mono" and a single stream.
 * That doesn't work for some USB devices (a Platronics headset), so
 * mono in, virtual mono out, and mono in/out are also tried.
 *
 * 2: Unix/OSS: most cards are OK with real mono, and a single stream.
 * Except some.  For those, a single open with real mono will succeed,
 * but execution will fail.  Maybe others will open OK with a single
 * stream, and real mono, but fail later? Two stream mono is tried first,
 * since it reportedly provides better sound quality with ALSA
 * and Sound Blaster Live.
 *
 * The failure mode I saw with a volunteer was that reads/writes would
 * return -enodev (down in the portaudio code).  Bummer.
 *
 * Win32 works fine, in all cases, with a single stream and real mono,
 * so far.
 *
 * We could probably do this more cleanly, because there are still cases
 * where we will fail (i.e. if the user has only mono in and out on a Mac).
 *
 * */
static int pa_openstreams (struct iaxc_audio_driver *d )
{
	int err;

#ifdef LINUX
	err = pa_open(0, 1, 1) && /* two stream mono */
		pa_open(1, 1, 1) &&   /* one stream mono */
		pa_open(0, 0, 0);     /* two stream stereo */
#else
#ifdef MACOSX
	err = pa_open(1, 0, 0) &&  /* one stream stereo */
		pa_open(1, 1, 0) &&    /* one stream mono in stereo out */
		pa_open(1, 1, 1) &&    /* one stream mono */
		pa_open(0, 0, 0);      /* two stream stereo */
#else
	err = pa_open(1, 1, 1) &&  /* one stream mono */
		pa_open(1, 0, 0) &&    /* one stream stereo */
		pa_open(1, 1, 0) &&    /* one stream mono in stereo out */
		pa_open(0, 0, 0);      /* two stream stereo */
#endif /*MACOSX */
#endif /* LINUX */

	if (err)
	{
		handle_paerror(err, "Unable to open streams");
		return -1;
	}
	return 0;
}

static int pa_openauxstream (struct iaxc_audio_driver *d )
{
	PaError err;

	struct PaStreamParameters ring_stream_params;

	// setup the ring parameters
	ring_stream_params.device = selectedRing;
	ring_stream_params.sampleFormat = paInt16;
	ring_stream_params.suggestedLatency =
		Pa_GetDeviceInfo(selectedRing)->defaultLowOutputLatency;
	ring_stream_params.hostApiSpecificStreamInfo = NULL;

	// first we'll try mono
	ring_stream_params.channelCount = 1;

	err = Pa_OpenStream(&aStream,
			NULL,
			&ring_stream_params,
			iaxci_sample_rate,
			paFramesPerBufferUnspecified, //FEEBACK - unsure if appropriate
			paNoFlag,
			(PaStreamCallback *)pa_aux_callback,
			NULL);

	if ( err != paNoError )
	{
		// next we'll try virtual mono (stereo)
		ring_stream_params.channelCount = 1;

		err = Pa_OpenStream(&aStream,
				NULL,
				&ring_stream_params,
				iaxci_sample_rate,
				paFramesPerBufferUnspecified, //FEEBACK - unsure if appropriate
				paNoFlag,
				(PaStreamCallback *)pa_aux_callback,
				NULL);
	}

	// mmok, failure...
	if ( err != paNoError )
	{
		// fprintf(stderr, "Failure opening ring device with params: id: %d, output %d, default output %d\n",
		// selectedRing, selectedOutput, Pa_GetDefaultOutputDevice());

		handle_paerror(err, "opening separate ring stream");
		return -1;
	}

	// Determine whether virtual mono is being used
	virtualMonoRing = ring_stream_params.channelCount - 1;

	return 0;
}

static int pa_start(struct iaxc_audio_driver *d)
{
	if ( running )
		return 0;

	/* re-open mixers if necessary */
	if ( iMixer )
	{
		Px_CloseMixer(iMixer);
		iMixer = NULL;
	}

	if ( oMixer )
	{
		Px_CloseMixer(oMixer);
		oMixer = NULL;
	}

	/* flush the ringbuffers */
	rb_InitializeRingBuffer(&inRing, INRBSZ, inRingBuf);
	rb_InitializeRingBuffer(&outRing, OUTRBSZ, outRingBuf);

	if ( pa_openstreams(d) )
		return -1;

	if ( selectedInput == paNoDevice )
	{
		if ( Pa_StartStream(oStream) != paNoError )
		return -1;

		oMixer = Px_OpenMixer(oStream, 0);
	}
	else
	{
		if ( Pa_StartStream(iStream) != paNoError )
			return -1;

	iMixer = Px_OpenMixer(iStream, 0);

	if ( !oneStream )
	{
			if ( Pa_StartStream(oStream) != paNoError )
		{
			Pa_StopStream(iStream);
			return -1;
			}
			oMixer = Px_OpenMixer(oStream, 0);
		}
	}

	if ( selectedRing != selectedOutput )
	{
		auxStream = 1;
	}
	else
	{
		auxStream = 0;
	}

	if ( auxStream )
	{
		pa_openauxstream(d);
		if ( Pa_StartStream(aStream) != paNoError )
		{
			auxStream = 0;
		}
	}

	/* select the microphone as the input source */
	if ( iMixer != NULL && !mixers_initialized )
	{
		/* First, select the "microphone" device, if it's available */
		/* try the new method, reverting to the old if it fails */
		if ( Px_SetCurrentInputSourceByName( iMixer, "microphone" ) != 0 )
		{
			int n = Px_GetNumInputSources( iMixer ) - 1;
			for ( ; n > 0; --n )
			{
				if ( !strcasecmp("microphone",
						Px_GetInputSourceName(iMixer, n)) )
				{
					Px_SetCurrentInputSource( iMixer, n );
				}
			}
		}

		/* try to set the microphone boost -- we just turn off this
		 * "boost" feature, because it often leads to clipping, which
		 * we can't fix later -- but we can deal with low input levels
		 * much more gracefully */
		Px_SetMicrophoneBoost( iMixer, 0 );

		/* If the input level is very low, raise it up a bit.
		 * Otherwise, AGC cannot detect speech, and cannot adjust
		 * levels */
		if ( pa_input_level_get(d) < 0.5f )
			pa_input_level_set(d, 0.6f);
		mixers_initialized = 1;
	}

	running = 1;
	return 0;
}

static int pa_stop (struct iaxc_audio_driver *d )
{
	PaError err;

	if ( !running )
		return 0;

	if ( sounds )
		return 0;

		err = Pa_AbortStream(oStream);
		err = Pa_CloseStream(oStream);

	if ( !oneStream )
	{
		err = Pa_AbortStream(iStream);
		err = Pa_CloseStream(iStream);
	}

	if ( auxStream )
	{
		err = Pa_AbortStream(aStream);
		err = Pa_CloseStream(aStream);
	}

	running = 0;
	return 0;
}

/* Mihai: apparently nobody loves this function. Some actually hate it.
 * I bet if it's gone, no one will miss it.  Such a cold, cold world!
static void pa_shutdown()
{
	CloseAudioStream( oStream );
	if(!oneStream) CloseAudioStream( iStream );
	if(auxStream) CloseAudioStream( aStream );
}
*/

static void handle_paerror(PaError err, char * where)
{
	fprintf(stderr, "PortAudio error at %s: %s\n", where,
			Pa_GetErrorText(err));
}

static int pa_input(struct iaxc_audio_driver *d, void *samples, int *nSamples)
{
	int bytestoread;

	bytestoread = *nSamples * sizeof(SAMPLE);

	/* we don't return partial buffers */
	if ( rb_GetRingBufferReadAvailable(&inRing) < bytestoread )
	{
		*nSamples = 0;
		return 0;
	}

	rb_ReadRingBuffer(&inRing, samples, bytestoread);

	return 0;
}

static int pa_output(struct iaxc_audio_driver *d, void *samples, int nSamples)
{
	int bytestowrite = nSamples * sizeof(SAMPLE);
	int outRingLen;

	outRingLen = rb_GetRingBufferReadAvailable(&outRing);
	outRingLenAvg = (outRingLenAvg * 9 + outRingLen ) / 10;

	/* if we've got a big output buffer, drop this */
	if (outRingLen > (int)RBOUTTARGET_BYTES &&
			outRingLenAvg > (int)RBOUTTARGET_BYTES)
	{
	  //fprintf(stderr, "*O*");
	  return outRingLen/2;
	}

	//if(rb_GetRingBufferWriteAvailable(&outRing) < bytestowrite)
	//	fprintf(stderr, "O");

	rb_WriteRingBuffer(&outRing, samples, bytestowrite);

	return (outRingLen + bytestowrite)/2;

}

static int pa_select_devices(struct iaxc_audio_driver *d, int input,
		int output, int ring)
{
	selectedInput = input;
	selectedOutput = output;
	selectedRing = ring;
	if ( running )
	{
		/* stop/start audio, in order to switch devices */
		pa_stop(d);
		pa_start(d);
	}
	else
	{
		/* start/stop audio, in order to initialize mixers and levels */
		pa_start(d);
		pa_stop(d);
	}
	return 0;
}

static int pa_selected_devices(struct iaxc_audio_driver *d, int *input,
		int *output, int *ring)
{
	*input = selectedInput;
	*output = selectedOutput;
	*ring = selectedRing;
	return 0;
}

static int pa_destroy(struct iaxc_audio_driver *d)
{
	if( iMixer )
	{
		Px_CloseMixer(iMixer);
		iMixer = NULL;
	}
	if ( oMixer )
	{
		Px_CloseMixer(oMixer);
		oMixer = NULL;
	}
	if ( d )
	{
		if ( d->devices )
		{
			free(d->devices);
			d->devices= NULL;
		}
	}
	return Pa_Terminate();
}

static float pa_input_level_get(struct iaxc_audio_driver *d)
{
	/* iMixer should be non-null if we using either one or two streams */
	if ( !iMixer )
		return -1;

	/* make sure this device supports input volume controls */
	if ( Px_GetNumInputSources( iMixer ) == 0 )
		return -1;

	return Px_GetInputVolume(iMixer);
}

static float pa_output_level_get(struct iaxc_audio_driver *d)
{
	PxMixer *mix;

	/* oMixer may be null if we're using one stream,
	   in which case, iMixer should not be null,
	   if it is, return an error */

	if ( oMixer )
		mix = oMixer;
	else if ( iMixer )
		mix = iMixer;
	else
		return -1;

	/* prefer the pcm output, but default to the master output */
	if ( Px_SupportsPCMOutputVolume(mix) )
		return Px_GetPCMOutputVolume(mix);
	else
		return Px_GetMasterVolume(mix);
}

static int pa_input_level_set(struct iaxc_audio_driver *d, float level)
{
	/* make sure this device supports input volume controls */
	if ( !iMixer || Px_GetNumInputSources(iMixer) == 0 )
		return -1;

	Px_SetInputVolume(iMixer, level);

	return 0;
}

static int pa_output_level_set(struct iaxc_audio_driver *d, float level)
{
	PxMixer *mix;

	if ( oMixer )
		mix = oMixer;
	else if ( iMixer )
		mix = iMixer;
	else
		return -1;

	/* prefer the pcm output, but default to the master output */
	if ( Px_SupportsPCMOutputVolume(mix) )
		Px_SetPCMOutputVolume(mix, level);
	else
		Px_SetMasterVolume(mix, level);

	return 0;
}

static int pa_mic_boost_get(struct iaxc_audio_driver* d)
{
	if ( !iMixer )
		return -1;

	return Px_GetMicrophoneBoost(iMixer);
}

int pa_mic_boost_set(struct iaxc_audio_driver* d, int enable)
{
	if ( !iMixer )
		return -1;

	return Px_SetMicrophoneBoost(iMixer, enable);
}

/* initialize audio driver */
static int _pa_initialize (struct iaxc_audio_driver *d, int sr)
{
	PaError  err;

	iaxci_sample_rate = sr;

	/* initialize portaudio */
	if ( paNoError != (err = Pa_Initialize()) )
	{
		iaxci_usermsg(IAXC_TEXT_TYPE_ERROR, "Failed Pa_Initialize");
		return err;
	}

	/* scan devices */
	scan_devices(d);

	/* setup methods */
	d->initialize = pa_initialize;
	d->destroy = pa_destroy;
	d->select_devices = pa_select_devices;
	d->selected_devices = pa_selected_devices;
	d->start = pa_start;
	d->stop = pa_stop;
	d->output = pa_output;
	d->input = pa_input;
	d->input_level_get = pa_input_level_get;
	d->input_level_set = pa_input_level_set;
	d->output_level_get = pa_output_level_get;
	d->output_level_set = pa_output_level_set;
	d->play_sound = pa_play_sound;
	d->stop_sound = pa_stop_sound;
	d->mic_boost_get = pa_mic_boost_get;
	d->mic_boost_set = pa_mic_boost_set;

	/* setup private data stuff */
	selectedInput  = Pa_GetDefaultInputDevice();
	selectedOutput = Pa_GetDefaultOutputDevice();
	selectedRing   = Pa_GetDefaultOutputDevice();
	sounds = NULL;
	MUTEXINIT(&sound_lock);

	rb_InitializeRingBuffer(&inRing, INRBSZ, inRingBuf);
	rb_InitializeRingBuffer(&outRing, OUTRBSZ, outRingBuf);

	running = 0;

	return 0;
}

/* standard initialization:  Do the normal initialization, and then
   also initialize mixers and levels */
int pa_initialize(struct iaxc_audio_driver *d, int sr)
{
	_pa_initialize(d, sr);

	/* TODO: Kludge alert. We only do the funny audio start-stop
	 * business if iaxci_audio_output_mode is not set. This is a
	 * hack to allow certain specific users of iaxclient to avoid
	 * certain problems associated with portaudio initialization
	 * hitting a deadlock condition.
	 */
	if ( iaxci_audio_output_mode )
		return 0;

	/* start/stop audio, in order to initialize mixers and levels */
	pa_start(d);
	pa_stop(d);

	return 0;
}

/* alternate initialization:  delay mixer/level initialization until
   we actually start the device.  This is somewhat useful when you're about to start
   the device as soon as you've initialized it, and want to avoid the time it
   takes to start/stop the device before starting it again */
int pa_initialize_deferred(struct iaxc_audio_driver *d, int sr)
{
	_pa_initialize(d, sr);
	return 0;
}

