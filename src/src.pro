QT += core gui network webkit

TARGET = infoscreen

# qxmpp
CONFIG += link_pkgconfig
PKGCONFIG += qxmpp

SOURCES += \
    main.cpp \
    mainapplication.cpp \
    userinterface.cpp \
    browser.cpp \
    networkinterface.cpp

HEADERS += \
    mainapplication.h \
    userinterface.h \
    qexception.h \
    browser.h \
    networkinterface.h
