find_package(Protobuf COMPONENTS protoc)
macro(pack_protogen)
    cmake_parse_arguments(arg
        ""
        "TARGET;WORKDIR"
        "PROTO"
        ${ARGN}
    )

    if (NOT Protobuf_PROTOC_EXECUTABLE)
        message(FATAL_ERROR "Protobuf compiler was not found, please install `protobuf-compiler`")
    endif()
    add_dependencies(${arg_TARGET} protoc-gen-pack)

    set(protoc ${Protobuf_PROTOC_EXECUTABLE})
    set(plugin ${PACK_BIN_DIR}/protoc-gen-pack)
    if (PACK_BIN_DIR)
        set(plugin ${PACK_BIN_DIR}/protoc-gen-pack)
    else()
        set(plugin ${CMAKE_BINARY_DIR}/pack/protoc/protoc-gen-pack)
    endif()

    foreach(proto ${arg_PROTO})
        get_filename_component(outDir ${proto} DIRECTORY)
        get_filename_component(abs ${proto} ABSOLUTE)
        if (NOT arg_WORKDIR)
            get_filename_component(inc ${abs} DIRECTORY)
        else()
            get_filename_component(inc ${arg_WORKDIR} ABSOLUTE)
            set(outDir ${arg_WORKDIR})
            target_include_directories(${arg_TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${arg_WORKDIR})
        endif()

        get_filename_component(genName ${proto} NAME_WE)
        get_filename_component(getPath ${proto} DIRECTORY)

        set(result ${CMAKE_CURRENT_BINARY_DIR}/${getPath}/${genName}.h)

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${outDir})

        add_custom_command(
            OUTPUT  ${result}
            COMMAND ${protoc} --plugin=${plugin} -I ${inc} --pack_out=${CMAKE_CURRENT_BINARY_DIR}/${outDir} ${abs}
            DEPENDS ${plugin} ${proto}
        )
        target_sources(${arg_TARGET} PRIVATE ${result})
        target_sources(${arg_TARGET} PRIVATE ${proto})
    endforeach()

    message(STATUS "Generate protos for target ${arg_TARGET}")
endmacro()
