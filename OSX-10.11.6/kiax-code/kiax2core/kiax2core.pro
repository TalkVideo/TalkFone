TEMPLATE = lib

INCLUDEPATH +=./includes
CORELIBDIR =../kiax2core

win32 {
LIBSDIR +=./libs-windows
LIBS += $$LIBSDIR/iaxclient.dll $$LIBSDIR/sqlite3.dll -llibcurl
DEFINES += WIN32DEP
}

linux-g++ {
# Uncomment if you want dynamic linking
# LIBSDIR +=./libs-linux
# LIBS += $$LIBSDIR/libiaxclient.so.1.0.2 $$LIBSDIR/libsqlite3.so.0.8.6
LIBSDIR += $$CORELIBDIR/static-libs-linux
LIBS += $$LIBSDIR/libiaxclient.a $$CORELIBDIR/libkiax2core.a $$LIBSDIR/libjson.a $$LIBSDIR/libspeexdsp.a $$LIBSDIR/libspeex.a $$LIBSDIR/libportaudio.a $$LIBSDIR/libgsm.a $$LIBSDIR/libsqlite3.a /usr/lib/libasound.so.2

DEFINES += LINUXDEP
}

macx {
LIBSDIR +=./libs-macx
LIBS += $$LIBSDIR/libiaxclient.dylib $$LIBSDIR/libsqlite3.dylib
DEFINES += MACXDEP
}

# Comment if you want dynamic lib
# CONFIG += staticlib

# comment if you don't have hold() in your iaxclient
# DEFINES += IAXCLIENT_HOLD_HACK

# KIAXOPT += dottel
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
    CONFIG += dll debug
}


# install
target.path = ../VoIPMixCoreLib/libs
sources.files = $$SOURCES $$HEADERS *.pro 
sources.path = ../VoIPMixCoreLib
INSTALLS += target
