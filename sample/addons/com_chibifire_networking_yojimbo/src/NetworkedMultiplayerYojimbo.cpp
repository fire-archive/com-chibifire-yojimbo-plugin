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

void NetworkedMultiplayerYojimbo::close_connection () {
}

int NetworkedMultiplayerYojimbo::create_client (String ip, int port, int in_bandwidth, int out_bandwidth) {
    return 0;
}

int NetworkedMultiplayerYojimbo::create_server (int port, int max_clients, int in_bandwidth, int out_bandwidth) {
    return 0;
}

void NetworkedMultiplayerYojimbo::set_bind_ip (String ip) {
}

int NetworkedMultiplayerYojimbo::get_connection_status() const {
    return 0;
}

int NetworkedMultiplayerYojimbo::get_packet_peer() const {
    return 0;
}

int NetworkedMultiplayerYojimbo::get_unique_id() const {
    return 0;
}

void NetworkedMultiplayerYojimbo::poll() {
}

void NetworkedMultiplayerYojimbo::set_target_peer(int id) {
}

int NetworkedMultiplayerYojimbo::get_available_packet_count() const {
    return 0;
}

PoolByteArray NetworkedMultiplayerYojimbo::get_packet() {
    return PoolByteArray();
}

int NetworkedMultiplayerYojimbo::get_packet_error() const {
    return 0;
}

Variant NetworkedMultiplayerYojimbo::get_var() {
    return Variant();
}

int NetworkedMultiplayerYojimbo::put_packet(PoolByteArray buffer) {
    return 0;
}

int NetworkedMultiplayerYojimbo::put_var(Variant var) {
    return 0;
}

void NetworkedMultiplayerYojimbo::_register_methods() {
    register_method("_get_extensions", &NetworkedMultiplayerYojimbo::get_extensions);
    register_method("close_connection", &NetworkedMultiplayerYojimbo::close_connection);
    register_method("create_client", &NetworkedMultiplayerYojimbo::create_client);
    register_method("create_server", &NetworkedMultiplayerYojimbo::create_server);
    register_method("set_bind_ip", &NetworkedMultiplayerYojimbo::set_bind_ip);
    register_method("get_connection_status", &NetworkedMultiplayerYojimbo::get_connection_status);
    register_method("get_packet_peer", &NetworkedMultiplayerYojimbo::get_packet_peer);
    register_method("get_unique_id", &NetworkedMultiplayerYojimbo::get_unique_id);
    register_method("poll", &NetworkedMultiplayerYojimbo::poll);
    register_method("set_target_peer", &NetworkedMultiplayerYojimbo::set_target_peer);
    register_method("get_available_packet_count", &NetworkedMultiplayerYojimbo::get_available_packet_count);
    register_method("get_packet", &NetworkedMultiplayerYojimbo::get_packet);
    register_method("get_packet_error", &NetworkedMultiplayerYojimbo::get_packet_error);
    register_method("get_var", &NetworkedMultiplayerYojimbo::get_var);  
    register_method("put_packet", &NetworkedMultiplayerYojimbo::put_packet);
    register_method("put_var", &NetworkedMultiplayerYojimbo::put_var);
}
