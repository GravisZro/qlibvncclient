VERSION = 0.1

QT += core gui network

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CFLAGS   += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-variable

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets testlib} else {
    CONFIG += qtestlib
}

TARGET = qvncviewer2

TEMPLATE = app

DEFINES += QVNCVIEWER_VERSION=$$VERSION

INCLUDEPATH += common src



unix {
    PACKAGES = zlib libjpeg libvncclient
    for(PACKAGE, PACKAGES) {
        QMAKE_CXXFLAGS += $$system(pkg-config --cflags $${PACKAGE})
        LIBS += $$system(pkg-config --libs $${PACKAGE})
    }

    isEmpty(INSTALL_BIN_PATH) {
        target.path = /usr/local/bin
    } else {
        target.path = $$INSTALL_BIN_PATH
    }
    INSTALLS += target
    LIBS += -lgcrypt
}

win32 {
    LIBS += -lws2_32
}

SOURCES += \
    src/main.cpp \
    src/rfbclientwapper.cpp \
    src/mainwindow.cpp \
    src/surfacewidget.cpp

HEADERS += \
    src/macros.h \
    src/qt2keysum.h \
    src/rfbclientwapper.h \
    src/mainwindow.h \
    src/surfacewidget.h
