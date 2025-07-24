template = app
CONFIG += qt
CORELIBDIR = ../kiax2core
INCLUDEPATH += $$CORELIBDIR $$CORELIBDIR/includes $$CORELIBDIR/includes/json

# set USEWEBKIT to true if you want to link to webkit shared library
USEWEBKIT = false

linux-g++ {
# Uncomment if you want dynamic linking
# LIBSDIR += $$CORELIBDIR/libs-linux
# LIBS += -L/usr/lib -L/usr/lib4 -L/usr/local/lib -L /usr/local/lib64 -L$$CORELIBDIR -liaxclient -lkiax2core -ljson -lspeexdsp -lspeex -lportaudio -lsqlite3 -lasound 

LIBSDIR += $$CORELIBDIR/static-libs-linux
LIBS += $$LIBSDIR/libiaxclient.a $$CORELIBDIR/libkiax2core.a $$LIBSDIR/libjson.a $$LIBSDIR/libspeexdsp.a $$LIBSDIR/libspeex.a $$LIBSDIR/libportaudio.a $$LIBSDIR/libsqlite3.a -lcurl /usr/lib64/libasound.so.2 

LIBS += -lQt6Widgets -lQt6Core5Compat

# Uncomment if you want explicit dynamic linking
# LIBS += $$LIBSDIR/libiaxclient.so.1.0.2 $$LIBSDIR/libsqlite3.so.0.8.6 $$CORELIBDIR/libkiax2core.so.1.0.0 $$LIBSDIR/libjson.a $$LIBSDIR/libspeexdsp.so.1.4.0 $$LIBSDIR/libspeex.so.1.4.0

DEFINES += LINUXDEP
}

KIAXOPT += dottel

contains( KIAXOPT, dottel ): {
	message("Enabling dottel support..")
	DEFINES += USE_DOTTEL
	HEADERS += 	Kiax2DotTelDialog.h \
			../kiax2core/dottelutils/dottelutils.h \
			../kiax2core/dottelutils/CppDotTelutils.h
	SOURCES += 	Kiax2DotTelDialog.cpp

	FORMS += dotteldialog.ui
	INCLUDEPATH += $$CORELIBDIR/dottelutils

	linux-g++ {
		# LIBS += $$LIBSDIR/libldns.a -lcrypto
		LIBS += /usr/local/lib/libldns.a -lcrypto
	}
}

# comment if you dont have hold() in your iaxclient
# DEFINES += IAXCLIENT_HOLD_HACK

QT += network

contains( KIAXOPT, webforms ): {
	message("Using Webforms + WebKit..")
	QT += webkit
	DEFINES += USE_WEBFORMS
	HEADERS += 	Kiax2ChangePassDialog.cpp \
				Kiax2RegisterDialog.h
	SOURCES += 	Kiax2ChangePassDialog.cpp \
				Kiax2RegisterDialog.cpp
}

contains( KIAXOPT, webfeed ): {
	message("Using Webfeed +  WebKit..")
	QT += webkit
	DEFINES += USE_WEBFEED
}


FORMS += kiax2windowAlt.ui \
			accountdialog.ui \
			aboutdialog.ui \
			calldialog.ui \
			contactdialog.ui \
			contactlistitemwidget.ui \
			settingsdialog.ui \
			registerdialog.ui \
			loginscreen.ui
			
HEADERS += Kiax2MainWindow.h \
			Kiax2AboutDialog.h \
			Kiax2AccountDialog.h \
			Kiax2CallDialog.h \
			Kiax2ContactDialog.h \
			Kiax2ListWidgetItem.h \
			Kiax2ContactListItemWidget.h \
			Kiax2SettingsDialog.h \
			IAX2CallbackHandlerQt.h \
			Kiax2ConnectionMeter.h \
			Kiax2JSONBalance.h \
			Kiax2LoginScreen.h \
			Kiax2Application.h \
			Kiax2JSONUsersOnline.h \
			kiax2JSONNotification.h \
			Kiax2JSONServers.h \
			Kiax2DispatcherServer.h \
			Kiax2DispatcherThread.h \
			Kiax2Servers.h \
			Kiax2JSONSupernode.h \
			Kiax2CallAppearance.h \
			Kiax2CallDialogView.h \
			Kiax2CallButton.h  \
			../kiax2core/PConfiguration.h
			
			
SOURCES += Kiax2MainWindow.cpp \
			Kiax2AboutDialog.cpp \
			Kiax2AccountDialog.cpp \
			Kiax2CallDialog.cpp \
			Kiax2ContactDialog.cpp \
			Kiax2ListWidgetItem.cpp \
			Kiax2ContactListItemWidget.cpp \
			Kiax2SettingsDialog.cpp \
			IAX2CallbackHandlerQt.cpp \
			Kiax2ConnectionMeter.cpp \
			Kiax2JSONBalance.cpp \
			Kiax2LoginScreen.cpp \
			Kiax2Application.cpp \
			Kiax2JSONUsersOnline.cpp \
			Kiax2JSONNotification.cpp \
			Kiax2JSONServers.cpp \
			Kiax2DispatcherServer.cpp \
			Kiax2DispatcherThread.cpp \
			Kiax2Servers.cpp \
			Kiax2JSONSupernode.cpp \
			Kiax2CallAppearance.cpp \
			Kiax2CallDialogView.cpp \
			Kiax2CallButton.cpp

RC_FILE = kiax2.rc

TRANSLATIONS = kiax2_fr.ts \
			   kiax2_de.ts \
			   kiax2_es.ts

			
build_all:!build_pass {
    CONFIG -= build_all
}


# install
target.path = .
sources.files = $$SOURCES $$HEADERS $$FORMS *.pro 
sources.path = .
INSTALLS += target
