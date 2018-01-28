#! /bin/bash
export current=`pwd`
export godot=`pwd`/thirdparty/godot
cd $godot && scons p=x11 target=release_debug -j`nproc --all` use_llvm=yes use_lto=no gdnative_wrapper=yes deprecated=no && cd $current && $godot/bin/godot.x11.opt.tools.64.llvm --gdnative-generate-json-api thirdparty/cpp_bindings/godot_api.json && cd thirdparty/cpp_bindings/ && scons -j`nproc --all` godotbinpath="$godot/bin/godot.x11.opt.tools.64.llvm" headers="$godot/modules/gdnative/include" p=x11 generate_bindings=yes target=release && cd $current
