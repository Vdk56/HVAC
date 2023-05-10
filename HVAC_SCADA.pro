QT       += core gui serialbus charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    add.cpp \
    archive.cpp \
    autorization.cpp \
    main.cpp \
    mainwindow.cpp \
    nitification1.cpp \
    notification2.cpp \
    notification3.cpp \
    vent_unit.cpp \
    widget_chart.cpp \
    widget_scheme.cpp

HEADERS += \
    add.h \
    archive.h \
    autorization.h \
    mainwindow.h \
    nitification1.h \
    notification2.h \
    notification3.h \
    vent_unit.h \
    widget_chart.h \
    widget_scheme.h

FORMS += \
    add.ui \
    autorization.ui \
    mainwindow.ui \
    nitification1.ui \
    notification2.ui \
    notification3.ui \
    vent_unit.ui \
    widget_chart.ui \
    widget_scheme.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    imgs.qrc
