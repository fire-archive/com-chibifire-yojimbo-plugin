cc_binary(
    features = ["use_linker", "windows_export_all_symbols"],
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
    deps = ["yojimbo"]
)

cc_library(
    name = "sodium",
    srcs = [":sodium_version"] + glob(["thirdparty/libsodium/src/libsodium/**/*.c"]),
    hdrs = glob(["thirdparty/libsodium/src/libsodium/**/*.h"]) + 
    ["thirdparty/libsodium/src/libsodium/include/sodium/version.h"],
    includes = ["thirdparty/libsodium/src/libsodium/include/sodium",
    "thirdparty/libsodium/src/libsodium/include/private"],
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

cc_library(
    name = "netcode.io",
    srcs = glob(["thirdparty/netcode.io/netcode.c"]),
    includes = ["thirdparty/netcode.io", 
    "thirdparty/libsodium/src/libsodium/include"],
    defines = [
        "WINDOWS_BUILD=1"
    ],
    deps = ["sodium", "mbedtls"]
)

cc_library(
    name = "reliable.io",
    srcs = glob(["thirdparty/reliable.io/reliable.c"]),
    includes = ["thirdparty/reliable.io", 
    "thirdparty/netcode.io", 
    "thirdparty/libsodium/src/libsodium/include"],
    deps = ["netcode.io"]
)

cc_library(
    name = "yojimbo",
    srcs =  [ "thirdparty/yojimbo/yojimbo.h", 
    "thirdparty/yojimbo/yojimbo.cpp", 
    "thirdparty/yojimbo/tlsf/tlsf.h", 
    "thirdparty/yojimbo/tlsf/tlsf.c",
    "thirdparty/yojimbo/shared.h"],
    defines = [
        "WINDOWS_BUILD=1"
    ],
    deps = ["reliable.io"]
)

cc_binary(
    name = "yojimbo_secure_server",
    srcs =  ["thirdparty/yojimbo/secure_server.cpp"],
    deps = ["yojimbo"]
)

cc_binary(
    name = "yojimbo_secure_client",
    srcs =  ["thirdparty/yojimbo/secure_client.cpp"],
    deps = ["yojimbo"]
)
