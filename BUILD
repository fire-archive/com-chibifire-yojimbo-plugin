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

cc_library(
    name = "sodium",
    srcs = [":sodium_version"] + glob(["thirdparty/libsodium/src/libsodium/**/*.c"]),
    hdrs = glob(["thirdparty/libsodium/src/libsodium/**/*.h"]) + ["thirdparty/libsodium/src/libsodium/include/sodium/version.h"],
    includes = ["thirdparty/libsodium/src/libsodium/include/sodium",
    "thirdparty/libsodium/src/libsodium/include/private",
    "thirdparty/libsodium_version/include"],
    defines = [
        "SODIUM_STATIC"
    ],
    copts = ['-I$(GENDIR)/thirdparty/libsodium/src/libsodium/include/sodium/']
)

genrule(
    name = 'sodium_version',
    srcs = ['thirdparty/libsodium/src/libsodium/include/sodium/version.h.in'],
    outs = ['thirdparty/libsodium/src/libsodium/include/sodium/version.h'],
    cmd = """sed 's/@VERSION@/1.0.11/g
    s/@SODIUM_LIBRARY_VERSION_MAJOR@/10/g
    s/@SODIUM_LIBRARY_VERSION_MINOR@/1/g
    s/@SODIUM_LIBRARY_MINIMAL_DEF@//g
    ' $< > $@""",
)

cc_library(
    name = "mbedtls",
    srcs = glob(["thirdparty/mbedtls/library/*.c"]),
    hdrs = glob(["thirdparty/mbedtls/include/**/*.h"]),
    includes = ["thirdparty/mbedtls/include"],
    defines = [
        "WINDOWS_BUILD=1"
    ],
    copts = ['-I$(GENDIR)/thirdparty/libsodium/src/libsodium/include/sodium/']
)
