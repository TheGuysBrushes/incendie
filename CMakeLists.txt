project(Automate)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

add_definitions(-Wall)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Moteur) # must set ${Moteur_SRCS}
add_subdirectory(Display)# must set ${Display_SRCS}

set(Automate_SRCS ${TinyXML_SRCS} ${Moteur_SRCS} ${Display_SRCS} main.cpp)

#qt4_automoc(${Automate_SRCS})

add_executable(Automate ${Automate_SRCS})
target_link_libraries(Automate ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
