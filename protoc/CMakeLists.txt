cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

raven_target(exe protoc-gen-pack
    SOURCES
        src/main.cpp
        src/generator.h
        src/generator.cpp
        src/filegenerator.h
        src/filegenerator.cpp
        src/classgenerator.h
        src/classgenerator.cpp
        src/formatter.h
        src/utils.h
    CMAKE
        cmake/protogen.cmake
    PREPROCESSOR
        -DPROTOBUF_USE_DLLS
    USES
        protobuf::libprotobuf
        protobuf::libprotoc
)

