#-------------------------------------------------
#
# Project created by QtCreator 2017-01-15T09:43:02
#
#-------------------------------------------------

QT       += widgets

TARGET = dublicat
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    work.cpp \
    form_dubl.cpp \
    form_elem.cpp \
    aspectratiopixmaplabel.cpp \
    element.cpp \
    work_item.cpp \
    form_ddd.cpp

HEADERS  += mainwindow.h \
    work.h \
    form_dubl.h \
    form_elem.h \
    aspectratiopixmaplabel.h \
    element.h \
    work_item.h \
    form_ddd.h

FORMS    += \
    form_dubl.ui \
    form_elem.ui \
    mainwindow.ui \
    form_ddd.ui

unix {
    LIBS += -lavcodec -lavformat -lavutil -lavdevice
}

QMAKE_CXXFLAGS += -std=gnu++11

RESOURCES += \
    resources.qrc
