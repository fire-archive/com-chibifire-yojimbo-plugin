cc_binary(
    features = ["use_linker"],
    name = "com_chibifire_networking_yojimbo.dll",
    srcs = [
        "sample/addons/com_chibifire_networking_yojimbo/src/NetworkedMultiplayerYojimbo.cpp",
        "sample/addons/com_chibifire_networking_yojimbo/src/NetworkedMultiplayerYojimbo.h",
        "thirdparty/cpp_bindings/bin/godot_cpp_bindings.lib",
        "thirdparty/cpp_bindings/include/gdnative.hpp",
        "thirdparty/godot_headers/gdnative_api_struct.gen.h",
    ],
    includes = [
        "thirdparty/godot/modules/gdnative/include",
        "thirdparty/cpp_bindings/include/",
        "thirdparty/cpp_bindings/include/core",
    ],    
    linkstatic = 1,
    linkshared = 1,
    copts = ["-O2", "/MD"],
    linkopts = ["-NODEFAULTLIB:LIBCMT"],
    deps = []
)
