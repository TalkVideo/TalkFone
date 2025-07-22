bin_PROGRAMS = testcall

testcall_SOURCES = testcall.c

testcall_CPPFLAGS = -I$(top_srcdir)/lib
testcall_CFLAGS = $(PORTAUDIO_CFLAGS) $(SPEEX_CFLAGS)
testcall_LDFLAGS =
testcall_LDADD = $(top_builddir)/lib/libiaxclient.la \
		 $(PORTAUDIO_LIBS) $(SPEEX_LIBS)

if VIDEO
testcall_CFLAGS += $(THEORA_CFLAGS) $(VIDCAP_CFLAGS)
testcall_LDADD += $(THEORA_LIBS) $(VIDCAP_LIBS)
endif

EXTRA_DIST = testcall-jb.c

