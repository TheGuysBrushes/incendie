TEMPLATE = app
VERSION= 1.5

android {
    message("* Using settings for Android.")
    VERSION= 1
}

QT += core gui widgets
CONFIG += c++11
win32: {
    QMAKE_CXXFLAGS += -openmp
    QMAKE_LFLAGS += -openmp
    LIBS += -openmp
} else {
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS += -fopenmp
    LIBS += -fopenmp
}
#LIBS += -lpthread -lgomp

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
    win32:  TARGET = ..\Fire_debug_"$$VERSION"
    unix:   TARGET = ..\Fire_debug_"$$VERSION"

    message("Compiling Debug mode")
} else {
    message("I am at $$PWD")

    win32:  TARGET = ..\..\Releases\Fire_"$$VERSION"
    unix:   TARGET = ../Releases/Fire_"$$VERSION"

### Copie de l'exécutable dans un second dossier ###
#    message("Compiling to $$TARGET then copy executable to Release Directories")
#
#    # Copy executable in another directory after compilation
#    win32 { QMAKE_POST_LINK +=  $$QMAKE_COPY $$system_path($$TARGET).exe  $$system_path(..\Last_Release_dev_light\bin\Fire_"$$VERSION").exe }
#    unix  { QMAKE_POST_LINK +=  $$QMAKE_COPY $$system_path($$TARGET)      ../Last_Release_dev_light/bin/Fire_"$$VERSION"
#        # Create directory where executable will be copy
#        foldersTarget.target = create_folders
#        foldersTarget.commands =  mkdir -p ../Last_Release_dev_light/bin
#        QMAKE_EXTRA_TARGETS +=  foldersTarget
#        PRE_TARGETDEPS +=   $$foldersTarget.target
#    }
#    versionTarget.target = versioning
#    versionTarget.commands = @echo VERSION : $$VERSION
#    QMAKE_EXTRA_TARGETS +=  versionTarget lightTarget
#    PRE_TARGETDEPS +=   $$versionTarget.target
}

message(The project will be installed in $$DESTDIR)

### Input files ###
DEPENDPATH += . Display
INCLUDEPATH += ./Display

DISTFILES += ../README.md \
            ../.gitignore \
            fire.rc \
            ../.travis.yml \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

include(Engine/fire_engine.pri)

HEADERS += ./debug.h \
    ./Display/loadwindow.h \
    ./Display/firescreen.h \
    ./Display/firewidget.h \
    ./Display/fwelcome.h \
    ./Display/windcircle.h \
    ./Display/windwidget.h

SOURCES += ./main.cpp \
    ./Display/firescreen.cpp \
    ./Display/firewidget.cpp \
    ./Display/fwelcome.cpp \
    ./Display/loadwindow.cpp \
    ./Display/windcircle.cpp \
    ./Display/windwidget.cpp

TRANSLATIONS =  fire_fr.ts \
                fire_de.ts \
                fire_en.ts

RC_FILE = fire.rc

RESOURCES = all_resources.qrc

###  Compilation statique ###
# edit the file Path-To-Qt-SDK\qt_static\mkspecs\win32-g+.conf and add the bold (with * ) marked stuff
#QMAKE_CFLAGS_RELEASE = -Os -momit-leaf-frame-pointer
#QMAKE_LFLAGS = -static-libgcc
#win32{
#    QMAKE_LFLAGS += -static
#}
## Autre méthode compilation statique #
#DEFINES= QT_STATIC_BUILD
#edit Path-To-Qt-SDK\qt_static\qmake\Makefile.win32-g++
#LFLAGS = -static -static-libgcc
#edit Path-To-Qt-SDK\qt_static\src\3rdparty\webkit\WebKit.pri
#add 'CONFIG = staticlib* on the top
#CONFIG = staticlib*

## Crée par VS add-in
#DESTDIR = ./Release
#DEFINES += _WINDOWS WIN64 QT_WIDGETS_LIB
#LIBS += -L"../../../../../utils/postgresql/pgsql/lib" \
#    -L"../../../../../utils/my_sql/my_sql/lib" \
#    -lD:/Apps/Qt/5.7/msvc2015_64/lib/qtmain \
#    -lshell32 \
#    -lD:/Apps/Qt/5.7/msvc2015_64/lib/Qt5Widgets \
#    -lD:/Apps/Qt/5.7/msvc2015_64/lib/Qt5Gui \
#    -lD:/Apps/Qt/5.7/msvc2015_64/lib/Qt5Core
#MOC_DIR += release
#OBJECTS_DIR += release
#UI_DIR += ./GeneratedFiles
#RCC_DIR += ./GeneratedFiles

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
