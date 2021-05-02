QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/applcompiler.cpp \
    src/appleditor.cpp \
    src/atomcompiler.cpp \
    src/atomeditor.cpp \
    src/blockbuildutils.cpp \
    src/blockcompiler.cpp \
    src/blockeditor.cpp \
    src/blockinstance.cpp \
    src/compcompiler.cpp \
    src/compeditor.cpp \
    src/connectiondesigner.cpp \
    src/libraryexplorer.cpp \
    src/main.cpp \
    src/mainmenu.cpp \
    src/newfiledialog.cpp \
    src/porteditor.cpp \
    src/portwidget.cpp \
    src/speccache.cpp \
    src/tabeditor.cpp \
    src/xmlutils.cpp

HEADERS += \
    src/applcompiler.h \
    src/appleditor.h \
    src/atomcompiler.h \
    src/atomeditor.h \
    src/blockbuildutils.h \
    src/blockcompiler.h \
    src/blockeditor.h \
    src/blockinstance.h \
    src/blockspec.h \
    src/compcompiler.h \
    src/compeditor.h \
    src/connectiondesigner.h \
    src/libraryexplorer.h \
    src/mainmenu.h \
    src/newfiledialog.h \
    src/porteditor.h \
    src/portwidget.h \
    src/speccache.h \
    src/tabeditor.h \
    src/rapidxml/rapidxml.hpp \
    src/rapidxml/rapidxml_iterators.hpp \
    src/rapidxml/rapidxml_print.hpp \
    src/rapidxml/rapidxml_utils.hpp \
    src/xmlutils.h

FORMS += \
    src/appleditor.ui \
    src/atomeditor.ui \
    src/blockinstance.ui \
    src/compeditor.ui \
    src/libraryexplorer.ui \
    src/mainmenu.ui \
    src/newfiledialog.ui \
    src/porteditor.ui \
    src/portwidget.ui \
    src/tabeditor.ui

TRANSLATIONS += \
    src/icp2021_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/icp2021_en_US.ts
