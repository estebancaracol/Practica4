cmake_minimum_required(VERSION 3.16)

project(P41 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(P41 main.cpp
    enrutador.h enrutador.cpp
    red.h red.cpp)

include(GNUInstallDirs)
install(TARGETS P41
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
