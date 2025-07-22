bin_PROGRAMS = stresstest

stresstest_SOURCES = stresstest.c file.c file.h

stresstest_CPPFLAGS = -I$(top_srcdir)/lib
stresstest_CFLAGS = $(OGGZ_CFLAGS) \
		    $(THEORA_CFLAGS) \
		    $(PORTAUDIO_CFLAGS) \
		    $(SPEEX_CFLAGS) \
		    $(VIDCAP_CFLAGS)
stresstest_LDFLAGS =
stresstest_LDADD = $(top_builddir)/lib/libiaxclient.la \
		   $(OGGZ_LIBS) \
		   $(THEORA_LIBS) \
		   $(PORTAUDIO_LIBS) \
		   $(SPEEX_LIBS) \
		   $(VIDCAP_LIBS)



