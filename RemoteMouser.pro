QT += core websockets widgets
CONFIG += c++11

TARGET = RemoteMouser
TEMPLATE = app

include(lib/QrCode/Qt-QrCodeGenerator.pri)

SOURCES += \
    auth.cpp \
    jsonhelper.cpp \
    keyboard.cpp \
    main.cpp \
    mainwindow.cpp \
    mouse.cpp \
    networkhelper.cpp \
    totpgenerator.cpp \
    websocketserver.cpp

HEADERS += \
    InputEvent.h \
    auth.h \
    jsonhelper.h \
    keyboard.h \
    mainwindow.h \
    mouse.h \
    networkhelper.h \
    totpgenerator.h \
    websocketserver.h

# Platforma özgü kütüphaneler
win32:LIBS += -luser32
unix:LIBS += -lX11 -lXtst

FORMS += \
    form.ui

DISTFILES += \
    config.ini
