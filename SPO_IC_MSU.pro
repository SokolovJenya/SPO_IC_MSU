#-------------------------------------------------
#
# SPO IC MSU Project
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = SPO_IC_MSU
TEMPLATE = app


SOURCES += main.cpp\
        frmMain.cpp \
    msu_message.cpp \
    ic_msu_message.cpp \
    frmImitProperties.cpp \
    random.cpp

HEADERS  += frmMain.h \
    msu_message.h \
    ic_msu_message.h \
    frmImitProperties.h \
    ic_msu_net_properties.h \
    random.h

FORMS    += frmMain.ui \
    frmImitProperties.ui
