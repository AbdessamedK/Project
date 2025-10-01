cmake_minimum_required(VERSION 2.8)
project( tp1 )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCES
    main.cpp
    kernel.cpp
    InterfaceMain.cpp
    InterfaceKernel.cpp
    InterfaceMulti.cpp
    InterfaceSeuil.cpp
    InterfaceHysteresis.cpp
    Model.cpp
)

add_executable( tp1 ${SOURCES} )
target_link_libraries( tp1  ${OpenCV_LIBS})