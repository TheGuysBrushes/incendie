project(Automate)
cmake_minimum_required(VERSION 2.6)
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

# Add the include directories for the Qt 5 Widgets module to
# the compile lines.
include_directories(${Qt5Widgets_INCLUDE_DIRS } ${Qt5Core_INCLUDE_DIRS } ${Qt5Gui_INCLUDE_DIRS } ${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})


# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(-Wall ${Qt5Widgets_DEFINITIONS})

add_subdirectory(Moteur) # must set ${Moteur_SRCS}
add_subdirectory(Display)# must set ${Display_SRCS}

set(Automate_SRCS ${Moteur_SRCS} ${Display_SRCS} main.cpp)

#Version normale
add_executable(Automate ${Automate_SRCS})
target_link_libraries(Automate ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})
set_target_properties(Automate PROPERTIES LINK_FLAGS_DEBUG "-static")

##Version statique
#set(mylib ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})

##add_library(mylib requires_constexpr.cpp)
# #cxx_constexpr is a usage-requirement
#target_compile_features(mylib PUBLIC cxx_constexpr)

# #SRCS will be compiled with -std=gnu++11 on GNU for cxx_constexpr.
#add_executable(Automate ${Automate_SRCS})
#target_link_libraries(Automate mylib)

#target_compile_options(
#    Automate PRIVATE
#    "/W3 /nologo /EHsc"
#    "$<$<CONFIG:Debug>:/MTd /Od /Ob0 /Zi /RTC1 /DDEBUG /D_DEBUG>"
#    "$<$<CONFIG:Release>:/MT /O2 /Ob2 /DNDEBUG>"
#)



  if(BUILD_STATIC_EXECUTABLES)
    set(CMAKE_EXE_LINKER_FLAGS -static)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
    set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
    set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
    set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
    # Maybe this works as well, haven't tried yet.
    # set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
  else(BUILD_STATIC_EXECUTABLES)
    # Set RPATH to use for installed targets; append linker search path
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LOFAR_LIBDIR}")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif(BUILD_STATIC_EXECUTABLES)
