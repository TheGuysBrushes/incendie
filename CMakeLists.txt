project(AutomateFeu)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-Wall)

# set(CURSES_NEED_NCURSES TRUE)
# 
# set(CURSES_LIBRARY "/opt/lib/libncurses.so")
# set(CURSES_INCLUDE_PATH "/opt/include")
# 
# set( CMAKE_INCLUDE_PATH "/usr/include")
# set( CMAKE_LIBRARY_PATH "/usr/lib/i386-linux-gnu/libncurses.so")

set(AutomateFeu_SRCS vent.cpp arbre.cpp Essence.cpp coordonnee.cpp terminal.cpp main.cpp)
qt4_automoc(${AutomateFeu_SRCS})
add_executable(AutomateFeu cellule.cpp foret.cpp ${AutomateFeu_SRCS})
target_link_libraries(AutomateFeu ${QT_QTCORE_LIBRARY})
target_link_libraries(AutomateFeu "-lncurses")