QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fsutils.cpp \
    main.cpp \
    mainwindow.cpp \
    makedirdialog.cpp \
    panelview.cpp \
    selectdelegate.cpp

HEADERS += \
    fsutils.h \
    mainwindow.h \
    makedirdialog.h \
    panelview.h \
    selectdelegate.h

FORMS += \
    mainwindow.ui \
    makedirdialog.ui \
    panelview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

DISTFILES += \
    icons/go-home.ico \
    icons/stock_exit.ico
