cmake_minimum_required(VERSION 2.8)
project( tp2 )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCES
    main.cpp
    Model.cpp
    kernel.cpp
)

add_executable( tp2 ${SOURCES} )
target_link_libraries( tp2  ${OpenCV_LIBS})