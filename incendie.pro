TEMPLATE = app
VERSION= 1.4.3.1

QT += core gui widgets
CONFIG += c++11

message()
message(Qt version: $$[QT_VERSION])


#The first step is to enable dependency tracking in the library itself. To do this you must tell qmake to save information about the library:
#CONFIG += create_prl
#The second step in this process is to enable reading of this meta information in the applications that use the static library:
#CONFIG += link_prl

# Instructions for debug or release mode
#build_pass:CONFIG(debug, debug|release) {
#    unix: TARGET = $$join(TARGET,,,_debug)
#    else: TARGET = $$join(TARGET,,,d)
#}

build_pass:CONFIG(debug, debug|release) {
    DESTDIR = $$PWD\build
    TARGET = Debug\Fire_debug
#    DLLDESTDIR += $$quote(rbuild/release-Windows)
    message("Compiling Debug mode")
} else {
    DESTDIR = $$PWD\build

    win32:  TARGET = Last_Release\Fire_"$$VERSION"
    unix:   TARGET = ../Last_Release/Fire_"$$VERSION"
#    #DLLDESTDIR += $$quote(Last_Release)

    message("Compiling to $$TARGET then copy executable to Release Directories")

    # Copy executable in another directory after compilation
    win32 { QMAKE_POST_LINK +=  $$QMAKE_COPY $$system_path($$TARGET).exe  Last_Release_dev_light\bin\Fire_"$$VERSION".exe }
    unix  { QMAKE_POST_LINK +=  $$QMAKE_COPY $$system_path($$TARGET)      ../Last_Release_dev_light/bin/Fire_"$$VERSION"
        # Create directory where executable will be copy
        foldersTarget.target = create_folders
        foldersTarget.commands =  mkdir -p ../Last_Release_dev_light/bin
        QMAKE_EXTRA_TARGETS +=  foldersTarget
        PRE_TARGETDEPS +=   $$foldersTarget.target
    }

    versionTarget.target = versioning
    versionTarget.commands = @echo VERSION : $$VERSION

    QMAKE_EXTRA_TARGETS +=  versionTarget lightTarget
    PRE_TARGETDEPS +=   $$versionTarget.target
}

message(The project will be installed in $$DESTDIR)

# edit the file Path-To-Qt-SDK\qt_static\mkspecs\win32-g+.conf and add the bold (with * ) marked stuff
#QMAKE_CFLAGS_RELEASE = -Os -momit-leaf-frame-pointer
#QMAKE_LFLAGS = -static-libgcc
win32{
#    QMAKE_LFLAGS += -static
}

#DEFINES= QT_STATIC_BUILD
#edit Path-To-Qt-SDK\qt_static\qmake\Makefile.win32-g++
#LFLAGS = -static -static-libgcc
#edit Path-To-Qt-SDK\qt_static\src\3rdparty\webkit\WebKit.pri
#add 'CONFIG = staticlib* on the top
#CONFIG = staticlib*

# Input
DEPENDPATH += . Display Moteur TinyXML
INCLUDEPATH += ./Display ./Moteur
TRANSLATIONS =  incendie_fr.ts \
                incendie_de.ts \
                incendie_en.ts

RC_FILE = incendie.rc

DISTFILES += README.md \
            .gitignore \
            .travis.yml

HEADERS += debug.h \
           Display/firescreen.h \
           Display/firewidget.h \
           Display/fwelcome.h \
           Display/loadwindow.h \
           Display/windcircle.h \
           Display/windwidget.h \
           Moteur/actions.h \
           Moteur/arbre.h \
           Moteur/cellule.h \
           Moteur/cellulevide.h \
           Moteur/coordonnee.h \
           Moteur/essence.h \
           Moteur/foret.h \
           Moteur/loadprogress.h \
           Moteur/vent.h \
           TinyXML/tinystr.h \
           TinyXML/tinyxml.h

SOURCES += main.cpp \
           Display/firescreen.cpp \
           Display/firewidget.cpp \
           Display/fwelcome.cpp \
           Display/loadwindow.cpp \
           Display/windcircle.cpp \
           Display/windwidget.cpp \
           Moteur/arbre.cpp \
           Moteur/cellule.cpp \
           Moteur/coordonnee.cpp \
           Moteur/essence.cpp \
           Moteur/foret.cpp \
           Moteur/loadprogress.cpp \
           Moteur/vent.cpp \
           TinyXML/tinystr.cpp \
           TinyXML/tinyxml.cpp \
           TinyXML/tinyxmlerror.cpp \
           TinyXML/tinyxmlparser.cpp
