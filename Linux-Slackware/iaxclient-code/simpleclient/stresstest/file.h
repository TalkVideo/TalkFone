#ifndef __FILE_H__
#define __FILE_H__

#ifdef WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#include <ogg/ogg.h>

static const int SPEEX_FRAME_DURATION = 20;
static const int SPEEX_SAMPLING_RATE = 8000;

int load_ogg_file(const char *filename);

ogg_packet * get_next_audio_op(struct timeval now);
ogg_packet * get_next_video_op(struct timeval now);

int audio_is_eos();
int video_is_eos();

#endif
