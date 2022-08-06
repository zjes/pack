if (WITH_PROTOBUF)
    set(PROTOBUF_SRC
        protobuf.cpp
    )
endif()

raven_test_target(${PROJECT_NAME}
    SOURCES
        main.cpp
        value.cpp
        list.cpp
        binary.cpp
        enums.cpp
        map.cpp
        simple.cpp
        nested.cpp
        string.cpp
        child.cpp
        options.cpp
        variant.cpp
        json.cpp
        ${PROTOBUF_SRC}
    PREPROCESSOR
        -DCATCH_CONFIG_FAST_COMPILE
)

if (WITH_PROTOBUF)
    pack_protogen(
        TARGET ${PROJECT_NAME}-test
        PROTO
            examples/example.proto
    )
endif()
