cmake_minimum_required(VERSION 2.6)

add_definitions(-Wall)

include_directories( ${CMAKE_CURRENT_BINARY_DIR})

set(TinyXML_SRCS Engine/TinyXML/tinyxml.cpp Engine/TinyXML/tinystr.cpp Engine/TinyXML/tinyxmlerror.cpp Engine/TinyXML/tinyxmlparser.cpp)

set(Engine_FILES Engine/coordonnee.cpp Engine/vent.cpp Engine/cellule.cpp Engine/essence.cpp Engine/arbre.cpp Engine/loadprogress.cpp Engine/foret.cpp)
set(Engine_SRCS ${TinyXML_SRCS} ${Engine_FILES} PARENT_SCOPE)
