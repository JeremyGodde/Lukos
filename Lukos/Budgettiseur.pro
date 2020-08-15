QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT_XCB_NATIVE_PAINTING = 1

TARGET = "Lukos"

TEMPLATE = app
RC_ICONS = ../ressources/logo.ico
VERSION = 0.2
QMAKE_TARGET_DESCRIPTION = "Lukos Bugeting Application"
QMAKE_TARGET_COMPANY = "Jeremy Godde"
QMAKE_TARGET_COPYRIGHT = "GNU GPL 3"

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    principale.cpp \
    source/Calendar.cpp \
    source/Container.cpp \
    source/Event.cpp \
    source/Parameters.cpp \
    source/customClass.cpp

HEADERS += \
    header/Calendar.h \
    header/Container.h \
    header/Event.h \
    header/Exception.h \
    header/Parameters.h \
    header/customClass.h \
    principale.h

FORMS += \
    principale.ui

RESOURCES += ressources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
