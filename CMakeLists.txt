project(Automate)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Automate_SRCS coordonnee.cpp cellule.cpp Essence.cpp arbre.cpp foret.cpp firewidget.cpp moc_firewidget.cpp firescreen.cpp moc_firescreen.cpp main.cpp)
qt4_automoc(${Automate_SRCS})
add_executable(Automate ${Automate_SRCS})
target_link_libraries(Automate ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
