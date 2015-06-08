project(Automate)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

add_definitions(-Wall)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Moteur)
add_subdirectory(Display)

#set(Headers_SRCS Display/loadwindow.h Display/windcircle.h Display/windwidget.h Display/fwelcome.h Display/firewidget.h Display/firescreen.h)

qt4_automoc(${Headers_SRCS})

set(Moteur_SRCS Moteur/coordonnee.cpp Moteur/vent.cpp Moteur/cellule.cpp Moteur/essence.cpp Moteur/arbre.cpp Moteur/loadprogress.cpp Moteur/foret.cpp)

#set(Mocs_SRCS Display/mocs/moc_windcircle.cpp Display/mocs/moc_windwidget.cpp Display/mocs/moc_fwelcome.cpp Display/mocs/moc_firewidget.cpp Display/mocs/moc_firescreen.cpp) ${Mocs_SRCS}

#set(Display_SRCS Display/loadwindow.cpp Display/windcircle.cpp Display/windwidget.cpp Display/fwelcome.cpp Display/firewidget.cpp Display/firescreen.cpp)

set(Display_SRCS Display/loadwindow.cpp Display/windcircle.cpp Display/mocs/moc_windcircle.cpp Display/windwidget.cpp Display/mocs/moc_windwidget.cpp Display/fwelcome.cpp Display/mocs/moc_fwelcome.cpp Display/firewidget.cpp Display/mocs/moc_firewidget.cpp Display/firescreen.cpp Display/mocs/moc_firescreen.cpp)


set(Automate_SRCS ${Moteur_SRCS} ${Display_SRCS} main.cpp)

#qt4_automoc(${Automate_SRCS})

add_executable(Automate ${Automate_SRCS})
target_link_libraries(Automate ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
