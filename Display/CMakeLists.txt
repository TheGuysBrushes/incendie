cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

add_definitions(-Wall)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(mocs)

#set(Headers_SRCS Display/loadwindow.h Display/windcircle.h Display/windwidget.h Display/fwelcome.h Display/firewidget.h Display/firescreen.h)
# qt4_automoc(${Headers_SRCS})

#set(Mocs_SRCS Display/mocs/moc_windcircle.cpp Display/mocs/moc_windwidget.cpp Display/mocs/moc_fwelcome.cpp Display/mocs/moc_firewidget.cpp Display/mocs/moc_firescreen.cpp) ${Mocs_SRCS}

#set(Display_SRCS Display/loadwindow.cpp Display/windcircle.cpp Display/windwidget.cpp Display/fwelcome.cpp Display/firewidget.cpp Display/firescreen.cpp)

set(Display_MOCS Display/mocs/moc_windcircle.cpp Display/mocs/moc_windwidget.cpp Display/mocs/moc_fwelcome.cpp Display/mocs/moc_firewidget.cpp Display/mocs/moc_firescreen.cpp)
set(Display_SRCS ${Display_MOCS} Display/loadwindow.cpp Display/windcircle.cpp Display/windwidget.cpp Display/fwelcome.cpp Display/firewidget.cpp Display/firescreen.cpp)
set(Display_SRCS ${Display_SRCS} PARENT_SCOPE)

#qt4_automoc(${Display_SRCS})