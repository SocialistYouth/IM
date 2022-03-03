#-------------------------------------------------
#
# Project created by QtCreator 2022-03-02T09:47:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qq
TEMPLATE = app
INCLUDEPATH += ./net
INCLUDEPATH += ./mediator
LIBS += -lws2_32

SOURCES += main.cpp\
        dialog.cpp \
    mediator/INetMediator.cpp \
    mediator/TcpClientMediator.cpp \
    net/TcpClient.cpp \
    ckernel.cpp \
    logindialog.cpp

HEADERS  += dialog.h \
    mediator/INetMediator.h \
    mediator/TcpClientMediator.h \
    net/INet.h \
    net/packdef.h \
    net/TcpClient.h \
    ckernel.h \
    logindialog.h

FORMS    += dialog.ui \
    logindialog.ui

RESOURCES += \
    resource.qrc
