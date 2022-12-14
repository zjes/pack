cmake_minimum_required(VERSION 3.13)
cmake_policy(VERSION 3.13)

############################################################################################################################################

project(pack VERSION 1.0.0)

############################################################################################################################################

find_package(raven-cmake PATHS ${CMAKE_BINARY_DIR})

############################################################################################################################################

option(WITH_PROTOBUF "Using protobuf provider" OFF)
option(WITH_QTSTRING "Using qt string" OFF)

############################################################################################################################################

set(sources)
set(defs)
set(libs)

if (WITH_PROTOBUF)
    find_package(Protobuf QUIET)
    if (NOT Protobuf_FOUND OR NOT Protobuf_PROTOC_EXECUTABLE)
        message(FATAL_ERROR "You tried to compile with protobuf support, but protobuf was not found, please install \
            libprotobuf-dev \
            libprotoc-dev \
            protobuf-compiler ")
    endif()

    list(APPEND sources src/providers/protobuf.cpp)
    list(APPEND defs    -DWITH_PROTOBUF)
    list(APPEND libs    protobuf::libprotobuf)
endif()

if (WITH_QTSTRING)
    find_package(Qt5 COMPONENTS Core)
    list(APPEND defs -DWITH_QTSTRING -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_TO_ASCII)
    list(APPEND libs Qt5::Core)
endif()

############################################################################################################################################

raven_target(shared ${PROJECT_NAME}
    PUBLIC
        pack/types/string.h
        pack/types/list.h
        pack/types/map.h
        pack/types/value.h
        pack/types/enum.h
        pack/types/node.h
        pack/types/variant.h
        pack/types/binary.h

        pack/types/private/string.inl
        pack/types/private/list.inl
        pack/types/private/map.inl
        pack/types/private/value.inl
        pack/types/private/enum.inl
        pack/types/private/binary.inl
        pack/types/private/variant.inl

        pack/pack.h
        pack/attribute.h
        pack/options.h
        pack/types.h
        pack/serialization.h
        pack/visitor.h
        pack/magic-enum.h
        pack/utils.h
        pack/expected.h
        pack/convert.h
        pack/formatter.h

    SOURCES
        src/node.cpp
        src/utils.cpp
        src/attribute.cpp
        src/providers/yaml.cpp
        src/providers/json.cpp
        src/providers/utils.h
        src/providers/utils.cpp
        ${sources}
    PREPROCESSOR ${defs}
    USES
        yaml-cpp
        nlohmann_json::nlohmann_json
        ${libs}
    USES_PUBLIC
        fmt::fmt
)


############################################################################################################################################

if (WITH_PROTOBUF)
    add_subdirectory(protoc)
    include(protoc/cmake/protogen.cmake)
endif()

############################################################################################################################################

add_subdirectory(tests)

############################################################################################################################################

