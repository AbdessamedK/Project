cmake_minimum_required(VERSION 3.12...3.29)

project(mif11 LANGUAGES CXX)

find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt6)

add_executable(${PROJECT_NAME} src/main.cpp
        src/mesh_interface.h
        src/point.h
        src/convex.h
        src/algorithm.h
)

target_link_libraries(${PROJECT_NAME} PUBLIC CGAL::CGAL_Basic_viewer)
target_link_libraries(${PROJECT_NAME} PUBLIC /usr/local/lib/libgetfem.so)
