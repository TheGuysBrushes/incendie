project(Automate)
cmake_minimum_required(VERSION 2.6)
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

add_definitions(-Wall)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Moteur) # must set ${Moteur_SRCS}
add_subdirectory(Display)# must set ${Display_SRCS}

set(Automate_SRCS ${Moteur_SRCS} ${Display_SRCS} main.cpp)

add_executable(Automate ${Automate_SRCS})
target_link_libraries(Automate ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})