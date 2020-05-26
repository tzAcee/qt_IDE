QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG += console
CONFIG += warn_on

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
    DarkStyle.cpp \
    clang_compiler.cpp \
    debuggeredit.cpp \
    editor.cpp \
    explorer.cpp \
    highlighter.cpp \
    main.cpp \
    mainmenu.cpp \
    mainstatus.cpp \
    mainwindow.cpp \
    saver.cpp

HEADERS += \
    DarkStyle.h \
    DragAndDropModel.h \
    clang_compiler.h \
    debuggeredit.h \
    editor.h \
    explorer.h \
    highlighter.h \
    mainmenu.h \
    mainstatus.h \
    mainwindow.h \
    saver.h

FORMS += \
    mainwindow.ui

INCLUDEPATH +=
RESOURCES += \
            darkstyle.qrc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -LE:/EProgs/prebuilt_clang/LLVM/lib/ -llibclang

INCLUDEPATH += E:/EProgs/prebuilt_clang/LLVM/include
DEPENDPATH += E:/EProgs/prebuilt_clang/LLVM/include
