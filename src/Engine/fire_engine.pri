DEPENDPATH += Engine Engine/TinyXML
INCLUDEPATH += Engine

HEADERS += Engine/actions.h \
    Engine/arbre.h \
    Engine/cellule.h \
    Engine/cellulevide.h \
    Engine/coordonnee.h \
    Engine/essence.h \
    Engine/foret.h \
    Engine/vent.h \
    Engine/loadprogress.h \
    Engine/TinyXML/tinystr.h \
    Engine/TinyXML/tinyxml.h

SOURCES += Engine/arbre.cpp \
    Engine/cellule.cpp \
    Engine/coordonnee.cpp \
    Engine/essence.cpp \
    Engine/foret.cpp \
    Engine/loadprogress.cpp \
    Engine/vent.cpp \
    Engine/TinyXML/tinystr.cpp \
    Engine/TinyXML/tinyxml.cpp \
    Engine/TinyXML/tinyxmlerror.cpp \
    Engine/TinyXML/tinyxmlparser.cpp
