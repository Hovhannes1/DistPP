QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    agent.cpp \
    event.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    myagent.cpp \
    p2plink.cpp \
    world.cpp

HEADERS += \
    agent.h \
    event.h \
    mainwindow.h \
    message.h \
    myagent.h \
    p2plink.h \
    world.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
