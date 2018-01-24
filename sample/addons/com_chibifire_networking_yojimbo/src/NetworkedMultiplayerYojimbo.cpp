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
#include <OS.hpp>

#include "NetworkedMultiplayerYojimbo.h"
#include <Animation.hpp>
#include <Directory.hpp>
#include <NetworkedMultiplayerPeer.hpp>
#include <File.hpp>
#include <ProjectSettings.hpp>
#include <Ref.hpp>
#include <Vector3.hpp>
#include "thirdparty/yojimbo/yojimbo.h"
#include "thirdparty/yojimbo/shared.h"
// clang-format on

bool verboseOutput = false;

using namespace godot;
using namespace yojimbo;
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

Error NetworkedMultiplayerYojimbo::initialize_yojimbo() {
	if (!InitializeYojimbo()) {
		printf("error: failed to initialize Yojimbo!\n");
		return FAILED;
	}

	yojimbo_log_level(YOJIMBO_LOG_LEVEL_DEBUG);
	return OK;
}

void NetworkedMultiplayerYojimbo::close_connection() {
	server->Stop();
	if (server != nullptr) {
		delete server;
	}

	ShutdownYojimbo();
}

int NetworkedMultiplayerYojimbo::create_client(String ip, int port, int in_bandwidth, int out_bandwidth) {
	if (initialize_yojimbo() != OK) {
		return FAILED;
	}

	yojimbo::Matcher matcher(yojimbo::GetDefaultAllocator());

	Godot::print("connecting client (secure)");

	uint64_t clientId = 0;
	uint64_t ProtocolId = 0;
	random_bytes((uint8_t *)&clientId, 8);
	printf("client id is %.16" PRIx64 "\n", clientId);

	if (!matcher.Initialize()) {
		printf("error: failed to initialize matcher\n");
		return FAILED;
	}

	printf("requesting match from https://localhost:8080\n");

	matcher.RequestMatch(ProtocolId, clientId, false);

	if (matcher.GetMatchStatus() == MATCH_FAILED) {
		// printf( "\nRequest match failed. Is the matcher running? Please run \"premake5 matcher\" before you connect a secure client\n" );
		return 1;
	}

	uint8_t connectToken[ConnectTokenBytes];
	matcher.GetConnectToken(connectToken);

	// printf( "received connect token from matcher\n" );

	double time = OS::get_ticks_msec();

	ClientServerConfig config;
	config.protocolId = ProtocolId;

	Client client(GetDefaultAllocator(), Address("0.0.0.0"), config, adapter, time);

	Address serverAddress("127.0.0.1", port);

	client.Connect(clientId, connectToken);

	if (client.IsDisconnected()) {
		return FAILED;
	}

	char addressString[256];
	client.GetAddress().ToString(addressString, sizeof(addressString));
	printf("client address is %s\n", addressString);

	// signal( SIGINT, interrupt_handler );

	// Poll
	client.SendPackets();
	client.ReceivePackets();
	if (client.IsDisconnected()) {
		return FAILED;
	}

	client.AdvanceTime(OS::get_ticks_msec());

	if (client.ConnectionFailed()) {
		return FAILED;
	}
	// End poll
	client.Disconnect();
	return OK;
}

int NetworkedMultiplayerYojimbo::create_server(int port, int max_clients, int in_bandwidth, int out_bandwidth) {
	if (initialize_yojimbo() != OK) {
		return FAILED;
	}

	if (server != nullptr) {
		return FAILED;
	}

	double time = OS::get_ticks_msec();

	config.protocolId = ProtocolId;

	uint8_t privateKey[yojimbo::KeyBytes] = { 0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea,
		0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4,
		0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
		0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };

	server = new yojimbo::Server(yojimbo::GetDefaultAllocator(), privateKey, yojimbo::Address("127.0.0.1", port), config, adapter, time);

	Godot::print("starting server (secure)");
	server->Start(max_clients);

	return OK;
}

void NetworkedMultiplayerYojimbo::set_bind_ip(String ip) {
}

int NetworkedMultiplayerYojimbo::get_connection_status() const {
	return OK;
}

int NetworkedMultiplayerYojimbo::get_packet_peer() const {
	return OK;
}

int NetworkedMultiplayerYojimbo::get_unique_id() const {
	return OK;
}

void NetworkedMultiplayerYojimbo::poll() {
	if (!server->IsRunning()) {
		return;
	}
	server->SendPackets();
	server->ReceivePackets();
	server->AdvanceTime(OS::get_ticks_msec());
}

void NetworkedMultiplayerYojimbo::set_target_peer(int id) {
}

int NetworkedMultiplayerYojimbo::get_available_packet_count() const {
	return OK;
}

PoolByteArray NetworkedMultiplayerYojimbo::get_packet() {
	return PoolByteArray();
}

int NetworkedMultiplayerYojimbo::get_packet_error() const {
	return OK;
}

Variant NetworkedMultiplayerYojimbo::get_var() {
	return Variant();
}

int NetworkedMultiplayerYojimbo::put_packet(PoolByteArray buffer) {
	return OK;
}

int NetworkedMultiplayerYojimbo::put_var(Variant var) {
	return OK;
}

void NetworkedMultiplayerYojimbo::_register_methods() {
	register_method("close_connection", &NetworkedMultiplayerYojimbo::close_connection);
	register_method("create_client", &NetworkedMultiplayerYojimbo::create_client);
	register_method("create_client_yojimbo", &NetworkedMultiplayerYojimbo::create_client);
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

	register_signal<NetworkedMultiplayerYojimbo>("connection_failed");
	register_signal<NetworkedMultiplayerYojimbo>("connection_succeeded");
	register_signal<NetworkedMultiplayerYojimbo>("peer_connected");
	register_signal<NetworkedMultiplayerYojimbo>("server_disconnected");
}
