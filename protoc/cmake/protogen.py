import cmake

def generate(target, *protos):
    target.additionalScripts.append(
        """
include("{}/protoc/cmake/protogen.cmake")
pack_protogen(TARGET {}
PROTO
    {}
)
        """.format(cmake.Config.variables["CMAKE_CURRENT_LIST_DIR"], target.name, "\n".join(protos))
    )
    # compiler = cmake.Config.variables["Protobuf_PROTOC_EXECUTABLE"]

    # if compiler == "Protobuf_PROTOC_EXECUTABLE-NOTFOUND":
    #     raise Exception("Protobuf compiler was not found, please install `protobuf-compiler`")

