QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/atomeditor.cpp \
    src/blockcompiler.cpp \
    src/blockeditor.cpp \
    src/compeditor.cpp \
    src/connectiondesigner.cpp \
    src/libraryexplorer.cpp \
    src/main.cpp \
    src/mainmenu.cpp \
    src/newfiledialog.cpp \
    src/specstash.cpp \
    src/tabeditor.cpp \
    src/variableeditor.cpp \
    src/xmlutils.cpp

HEADERS += \
    src/atomeditor.h \
    src/blockcompiler.h \
    src/blockeditor.h \
    src/blockspec.h \
    src/compeditor.h \
    src/connectiondesigner.h \
    src/libraryexplorer.h \
    src/mainmenu.h \
    src/newfiledialog.h \
    src/specstash.h \
    src/tabeditor.h \
    src/rapidxml/rapidxml.hpp \
    src/rapidxml/rapidxml_iterators.hpp \
    src/rapidxml/rapidxml_print.hpp \
    src/rapidxml/rapidxml_utils.hpp \
    src/variableeditor.h \
    src/xmlutils.h

FORMS += \
    src/atomeditor.ui \
    src/compeditor.ui \
    src/libraryexplorer.ui \
    src/mainmenu.ui \
    src/newfiledialog.ui \
    src/tabeditor.ui \
    src/variableeditor.ui

TRANSLATIONS += \
    src/icp2021_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/icp2021_en_US.ts
