cmake_minimum_required(VERSION 2.6)

add_definitions(-Wall)

include_directories( ${CMAKE_CURRENT_BINARY_DIR})

set(TinyXML_SRCS TinyXML/tinyxml.cpp TinyXML/tinystr.cpp TinyXML/tinyxmlerror.cpp TinyXML/tinyxmlparser.cpp)

set(Moteur_SRCS Moteur/coordonnee.cpp Moteur/vent.cpp Moteur/cellule.cpp Moteur/essence.cpp Moteur/arbre.cpp Moteur/loadprogress.cpp Moteur/foret.cpp)
set(Moteur_SRCS ${TinyXML_SRCS} ${Moteur_SRCS} PARENT_SCOPE)
