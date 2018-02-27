bin_PROGRAMS = vtestcall
vtestcall_SOURCES = vtestcall.c

vtestcall_CPPFLAGS = -I$(top_srcdir)/lib
vtestcall_CFLAGS = $(SDL_CFLAGS) \
		   $(OGGZ_CFLAGS) \
		   $(PORTAUDIO_CFLAGS) \
		   $(SPEEX_CFLAGS) \
		   $(THEORA_CFLAGS) \
		   $(VIDCAP_CFLAGS)

vtestcall_LDADD = $(top_builddir)/lib/libiaxclient.la \
		  $(SDL_LIBS) \
		  $(OGGZ_LIBS) \
		  $(PORTAUDIO_LIBS) \
		  $(SPEEX_LIBS) \
		  $(THEORA_LIBS) \
		  $(VIDCAP_LIBS)

EXTRA_DIST =

