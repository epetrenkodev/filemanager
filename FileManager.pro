QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    action.cpp \
    fsutils.cpp \
    main.cpp \
    mainwindow.cpp \
    makedirdialog.cpp \
    panelview.cpp \
    propertiesdialog.cpp \
    selectdelegate.cpp \
    sortproxy.cpp

HEADERS += \
    action.h \
    fsutils.h \
    mainwindow.h \
    makedirdialog.h \
    panelview.h \
    propertiesdialog.h \
    selectdelegate.h \
    sortproxy.h

FORMS += \
    mainwindow.ui \
    makedirdialog.ui \
    panelview.ui \
    propertiesdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

DISTFILES += \
    icons/about.ico \
    icons/app.png \
    icons/copy.ico \
    icons/copy_to_folder.ico \
    icons/cut.ico \
    icons/delete.ico \
    icons/exit.ico \
    icons/go-home.ico \
    icons/help.ico \
    icons/home.ico \
    icons/move.ico \
    icons/new_folder.ico \
    icons/paste.ico \
    icons/stock_exit.ico
