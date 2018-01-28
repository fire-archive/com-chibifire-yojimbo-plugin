cc_library(
    name = "com_chibifire_networking_yojimbo",
    srcs = [
        "thirdparty/cpp_bindings/bin/godot_cpp_bindings.a",
    ],
    copts = [
        "-O2",
        "/MD",
    ],
    includes = [
        "thirdparty/cpp_bindings/include/",
        "thirdparty/cpp_bindings/include/core",
        "thirdparty/godot/modules/gdnative/include",
    ],
    linkstatic = 1,
    deps = ["networking_yojimbo"],
)

cc_library(
    name = "networking_yojimbo",
    srcs = [
        "sample/addons/com_chibifire_networking_yojimbo/src/NetworkedMultiplayerYojimbo.cpp",
        "sample/addons/com_chibifire_networking_yojimbo/src/NetworkedMultiplayerYojimbo.h",
        "thirdparty/cpp_bindings/include/GDNative.hpp",
        "thirdparty/godot_headers/gdnative_api_struct.gen.h",
        "thirdparty/yojimbo/shared.h",
        "thirdparty/yojimbo/yojimbo.h",
    ],
    copts = [
        "-O2",
        "/MD",
    ],
    features = [
        "use_linker",
        "windows_export_all_symbols",
    ],
    includes = [
        "thirdparty/cpp_bindings/include/",
        "thirdparty/cpp_bindings/include/core",
        "thirdparty/godot/modules/gdnative/include",
    ],
    deps = ["yojimbo"],
)

cc_library(
    name = "sodium",
    srcs = [":sodium_version"] + glob(["thirdparty/libsodium/src/libsodium/**/*.c"]),
    hdrs = glob(["thirdparty/libsodium/src/libsodium/**/*.h"]) + [
        "thirdparty/libsodium/src/libsodium/include/sodium/version.h",
    ],
    copts = ["-I$(GENDIR)/thirdparty/libsodium/src/libsodium/include/sodium/"],
    defines = [
        "SODIUM_STATIC",
    ],
    includes = [
        "thirdparty/libsodium/src/libsodium/include/private",
        "thirdparty/libsodium/src/libsodium/include/sodium",
    ],
)

genrule(
    name = "sodium_version",
    srcs = ["thirdparty/libsodium/src/libsodium/include/sodium/version.h.in"],
    outs = ["thirdparty/libsodium/src/libsodium/include/sodium/version.h"],
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
    copts = ["-I$(GENDIR)/thirdparty/libsodium/src/libsodium/include/sodium/"],
    defines = [
        "WINDOWS_BUILD=1",
    ],
    includes = ["thirdparty/mbedtls/include"],
)

cc_library(
    name = "netcode.io",
    srcs = glob(["thirdparty/netcode.io/netcode.c"]),
    defines = [
        "WINDOWS_BUILD=1",
    ],
    includes = [
        "thirdparty/libsodium/src/libsodium/include",
        "thirdparty/netcode.io",
    ],
    deps = [
        "mbedtls",
        "sodium",
    ],
)

cc_library(
    name = "reliable.io",
    srcs = glob(["thirdparty/reliable.io/reliable.c"]),
    includes = [
        "thirdparty/libsodium/src/libsodium/include",
        "thirdparty/netcode.io",
        "thirdparty/reliable.io",
    ],
    deps = ["netcode.io"],
)

cc_library(
    name = "yojimbo",
    srcs = [
        "thirdparty/yojimbo/shared.h",
        "thirdparty/yojimbo/tlsf/tlsf.c",
        "thirdparty/yojimbo/tlsf/tlsf.h",
        "thirdparty/yojimbo/yojimbo.cpp",
        "thirdparty/yojimbo/yojimbo.h",
    ],
    defines = [
        "WINDOWS_BUILD=1",
    ],
    deps = ["reliable.io"],
)

cc_binary(
    name = "yojimbo_secure_server",
    srcs = ["thirdparty/yojimbo/secure_server.cpp"],
    deps = ["yojimbo"],
)

cc_binary(
    name = "yojimbo_secure_client",
    srcs = ["thirdparty/yojimbo/secure_client.cpp"],
    deps = ["yojimbo"],
)
