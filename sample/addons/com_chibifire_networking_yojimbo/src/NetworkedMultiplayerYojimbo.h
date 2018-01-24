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

#pragma once

#include "thirdparty/yojimbo/yojimbo.h"
#include <Animation.hpp>
#include <ArrayMesh.hpp>
#include <NetworkedMultiplayerPeerGDNative.hpp>
#include <Node.hpp>
#include <Ref.hpp>
#include <Reference.hpp>
#include <String.hpp>
#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>

using namespace godot;

class NetworkedMultiplayerYojimbo : public GodotScript<NetworkedMultiplayerPeerGDNative> {
private:
	GODOT_CLASS(NetworkedMultiplayerYojimbo);
	Error initialize_yojimbo();
	yojimbo::ClientServerConfig config;
	yojimbo::Server *server;

public:
	NetworkedMultiplayerYojimbo() {
	}
	~NetworkedMultiplayerYojimbo() {
	}

	void close_connection();
	int create_client(String ip, int port, int in_bandwidth = 0, int out_bandwidth = 0);
	int create_server(int port, int max_clients = 32, int in_bandwidth = 0, int out_bandwidth = 0);
	void set_bind_ip(String ip);

	int get_connection_status() const;
	int get_packet_peer() const;
	int get_unique_id() const;
	void poll();
	void set_target_peer(int id);

	int get_available_packet_count() const;
	PoolByteArray get_packet();
	int get_packet_error() const;
	Variant get_var();
	int put_packet(PoolByteArray buffer);
	int put_var(Variant var);

	static void _register_methods();
};
