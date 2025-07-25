TEMPLATE = lib

INCLUDEPATH +=./includes
CORELIBDIR =../kiax2core

linux-g++ {

LIBSDIR += $$CORELIBDIR/static-libs-linux
LIBS += $$LIBSDIR/libiaxclient.a $$CORELIBDIR/libkiax2core.a $$LIBSDIR/libspeexdsp.a $$LIBSDIR/libspeex.a $$LIBSDIR/libportaudio.a $/usr/lib64/libsqlite3.so /usr/lib/libasound.so.2

# LIBS += $$LIBSDIR/libiaxclient.a $$CORELIBDIR/libkiax2core.a $$LIBSDIR/libjson.a $$LIBSDIR/libspeexdsp.a $$LIBSDIR/libspeex.a $$LIBSDIR/libportaudio.a $$LIBSDIR/libsqlite3.a /usr/lib/libasound.so.2

# Uncomment if you want explicit dynamic linking
# LIBSDIR +=./libs-linux
# LIBS += $$LIBSDIR/libiaxclient.so.1.0.2 $$LIBSDIR/libsqlite3.so.0.8.6


DEFINES += LINUXDEP
}


# Comment if you want dynamic lib
 CONFIG += staticlib

# comment if you dont have hold() in your iaxclient
# DEFINES += IAXCLIENT_HOLD_HACK

KIAXOPT += dottel
# KIAXOPT += remotecdr

contains( KIAXOPT, dottel ): {
        message("Enabling dottel support..")
        DEFINES += USE_DOTTEL
        HEADERS +=      dottelutils/dottelutils.h \
			dottelutils/CppDotTelutils.h
        SOURCES +=      dottelutils/dottelutils.c \
			dottelutils/CppDotTelUtils.cpp
        INCLUDEPATH += $$CORELIBDIR/includes/ldns
}

contains( KIAXOPT, remotecdr ): {
        message("Enabling remote CDR support..")
        DEFINES += REMOTE_CDR
        HEADERS += CDRModuleRemXml.h \
			HttpXmlFetcher.h
        SOURCES += HttpXmlFetcher.cpp \
		CDRModuleRemXml.cpp
}

HEADERS += APIBinder.h \
		CDR.h \
		ContactsModule.h \
		SignalingAPI.h \
		APIBinderDefault.h \
	    CDRAPI.h \
		ContactsModuleImpl.h \
		SignalingCallback.h \
		AbstractModule.h \
		CDRModule.h \
		Core.h \
		SignalingModule.h \
		AbstractService.h \
		CDRModuleImpl.h \
		Logger.h \
		SignalingModuleIAX2.h \
		Account.h \
		Configuration.h \
		NamingService.h \
		StorageAPI.h \
		AccountsAPI.h \
        ConfigurationParameter.h \
		PConfiguration.h \
		StorageService.h \
		AccountsModule.h \
		Contact.h \
		PConstants.h \
		AccountsModuleImpl.h \
		ContactsAPI.h \
		Serializable.h \
		IAX2CallbackHandler.h \
		ConfigurationAPI.h \
		ConfigurationModule.h \
		ConfigurationModuleImpl.h \
		tinyxml/tinystr.h \
		tinyxml/tinyxml.h
		
		

SOURCES += APIBinder.cpp \
        AccountsModuleImpl.cpp \
		Contact.cpp \
		Serializable.cpp \
		APIBinderDefault.cpp \
		CDR.cpp \
		ContactsModuleImpl.cpp \
		SignalingModuleIAX2.cpp	\
		AbstractModule.cpp \
		CDRModuleImpl.cpp \
		Core.cpp \
		StorageService.cpp \
		AbstractService.cpp \
		Configuration.cpp \
 		Logger.cpp \
		Account.cpp \
		ConfigurationParameter.cpp \
		NamingService.cpp \
		ConfigurationModuleImpl.cpp \
		tinyxml/tinystr.cpp \
		tinyxml/tinyxml.cpp \
		tinyxml/tinyxmlerror.cpp \
		tinyxml/tinyxmlparser.cpp
		
build_all:!build_pass {
    CONFIG -= build_all
}


# install
target.path = ../VoIPMixCoreLib/libs
sources.files = $$SOURCES $$HEADERS *.pro 
sources.path = ../VoIPMixCoreLib
INSTALLS += target
