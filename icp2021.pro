QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/atomeditor.cpp \
    src/blockcompiler.cpp \
    src/compeditor.cpp \
    src/libraryexplorer.cpp \
    src/main.cpp \
    src/mainmenu.cpp \
    src/newfiledialog.cpp \
    src/tabeditor.cpp

HEADERS += \
    src/atomeditor.h \
    src/blockcompiler.h \
    src/compeditor.h \
    src/libraryexplorer.h \
    src/mainmenu.h \
    src/newfiledialog.h \
    src/tabeditor.h

FORMS += \
    src/atomeditor.ui \
    src/compeditor.ui \
    src/libraryexplorer.ui \
    src/mainmenu.ui \
    src/newfiledialog.ui \
    src/tabeditor.ui

TRANSLATIONS += \
    src/icp2021_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/icp2021_en_US.ts
