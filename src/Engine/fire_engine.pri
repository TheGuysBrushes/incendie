HEADERS += Engine/actions.h \
    Engine/arbre.h \
    Engine/cellule.h \
    Engine/cellulevide.h \
    Engine/coordonnee.h \
    Engine/essence.h \
    Engine/foret.h \
    Engine/vent.h \
    Engine/loadprogress.h

SOURCES += Engine/arbre.cpp \
    Engine/cellule.cpp \
    Engine/coordonnee.cpp \
    Engine/essence.cpp \
    Engine/foret.cpp \
    Engine/loadprogress.cpp \
    Engine/vent.cpp

### Add TinyXML along project compilation
#HEADERS += Engine/TinyXML/tinystr.h \
#    Engine/TinyXML/tinyxml.h

#SOURCES += Engine/TinyXML/tinystr.cpp \
#    Engine/TinyXML/tinyxml.cpp \
#    Engine/TinyXML/tinyxmlerror.cpp \
#    Engine/TinyXML/tinyxmlparser.cpp


unix:!macx|win32: LIBS += -L$$PWD/TinyXML/lib/ -lTinyXML2

INCLUDEPATH += $$PWD/TinyXML/include
DEPENDPATH += $$PWD/TinyXML/include

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/TinyXML/lib/TinyXML2.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/TinyXML/lib/libTinyXML2.a
