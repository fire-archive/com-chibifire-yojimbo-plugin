// Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.
// Copyright (c) 2014-2017 Godot Engine contributors (cf. AUTHORS.md)

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// -- Godot Engine <https://godotengine.org>

// clang-format off
#include <Reference.hpp>
#include <Godot.hpp>

#include "NetworkedMultiplayerYojimbo.h"
#include <Animation.hpp>
#include <Directory.hpp>
#include <NetworkedMultiplayerPeer.hpp>
#include <File.hpp>
#include <ProjectSettings.hpp>
#include <Ref.hpp>
#include <Vector3.hpp>
// clang-format on

bool verboseOutput = false;

using namespace godot;
using godot::Directory;
using godot::Ref;

extern "C" void godot_gdnative_init(godot_gdnative_init_options *options) {
    Godot::gdnative_init(options);
}

extern "C" void godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
    Godot::gdnative_terminate(options);
}

extern "C" void godot_nativescript_init(void *handle) {
    Godot::nativescript_init(handle);
    register_class<NetworkedMultiplayerYojimbo>();
}

Array NetworkedMultiplayerYojimbo::get_extensions() const {
    PoolStringArray arr;
    arr.push_back("fbx");
    return Array(arr);
}

void NetworkedMultiplayerYojimbo::_register_methods() {
    register_method("_get_extensions", &NetworkedMultiplayerYojimbo::get_extensions);
}
