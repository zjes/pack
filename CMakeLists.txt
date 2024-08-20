cmake_minimum_required(VERSION 3.20)
cmake_policy(VERSION 3.20)

############################################################################################################################################

find_package(raven-cmake PATHS ${CMAKE_BINARY_DIR})

cxx_project(pack
    VERSION  1.0.0
    STANDARD 23
    FILE pack.project
)

