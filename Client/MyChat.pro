#-------------------------------------------------
#
# Project created by QtCreator 2021-10-08T20:23:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyChat
TEMPLATE = app


INCLUDEPATH += ./net
INCLUDEPATH += ./mediator

LIBS += -lws2_32

SOURCES += main.cpp\
        mychatdialog.cpp \
    net/TcpClient.cpp \
    mediator/TcpClientMediator.cpp \
    ckernel.cpp \
    mediator/INetMediator.cpp \
    logindialog.cpp \
    useritem.cpp

HEADERS  += mychatdialog.h \
    net/INet.h \
    net/packdef.h \
    net/TcpClient.h \
    mediator/INetMediator.h \
    mediator/TcpClientMediator.h \
    ckernel.h \
    logindialog.h \
    useritem.h

FORMS    += mychatdialog.ui \
    useritem.ui \
    logindialog.ui

RESOURCES += \
    resource.qrc
