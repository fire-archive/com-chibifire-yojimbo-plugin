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

#include <Animation.hpp>
#include <Directory.hpp>
#include <NetworkedMultiplayerPeer.hpp>
#include <File.hpp>
#include <ProjectSettings.hpp>
#include <Ref.hpp>
#include <Vector3.hpp>
#include <OS.hpp>
#include "thirdparty/yojimbo/yojimbo.h"
#include "thirdparty/yojimbo/shared.h"
#include "NetworkedMultiplayerYojimbo.h"
// clang-format on

bool verboseOutput = false;

using namespace godot;
using namespace yojimbo;
using godot::Directory;
using godot::Ref;

static const int UNRELIABLE_UNORDERED_CHANNEL = 0;
static const int RELIABLE_ORDERED_CHANNEL = 1;
const int MaxPacketSize = 16 * 1024;
const int MaxSnapshotSize = 8 * 1024;
const int MaxBlockSize = 64 * 1024;

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
	if (client != nullptr || server != nullptr) {
		return OK;
	}

	if (!InitializeYojimbo()) {
		Godot::print("error: failed to initialize Yojimbo!");
		return FAILED;
	}

	config.numChannels = 2;
	config.channel[UNRELIABLE_UNORDERED_CHANNEL].type = CHANNEL_TYPE_UNRELIABLE_UNORDERED;
	config.channel[RELIABLE_ORDERED_CHANNEL].type = CHANNEL_TYPE_RELIABLE_ORDERED;
	config.channel[RELIABLE_ORDERED_CHANNEL].blockFragmentSize = 1024;

	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);
	return OK;
}

void NetworkedMultiplayerYojimbo::set_log_level(int level) {
	yojimbo_log_level(level);
}

void NetworkedMultiplayerYojimbo::close_connection() {
	if (server != nullptr) {
		server->Stop();
	}

	if (client != nullptr) {
		client->Disconnect();
	}

	if (server != nullptr) {
		delete server;
	}
	if (client != nullptr) {
		delete client;
	}

	if (matcher != nullptr) {
		delete matcher;
	}

	if (server != nullptr || client != nullptr || matcher != nullptr) {
		ShutdownYojimbo();
		server = nullptr;
		client = nullptr;
		matcher = nullptr;
	}
}

int NetworkedMultiplayerYojimbo::create_client(String ip, int port, int in_bandwidth, int out_bandwidth) {
	if (initialize_yojimbo() != OK) {
		return FAILED;
	}

	if (client != nullptr) {
		return FAILED;
	}

	matcher = new yojimbo::Matcher(yojimbo::GetDefaultAllocator());

	Godot::print("connecting client (secure)");

	client_id = gen_unique_id_();

	char buffer[100];
	snprintf(buffer, 100, "client id is %.16" PRIx64, client_id);
	Godot::print(buffer);

	if (!matcher->Initialize()) {
		Godot::print("error: failed to initialize matcher");
		return FAILED;
	}

	Godot::print("requesting match from https://localhost:8080");

	matcher->RequestMatch(ProtocolId, client_id, false);

	if (matcher->GetMatchStatus() == MATCH_FAILED) {
		Godot::print("Request match failed. Is the matcher running? Please run matcher before you connect a secure client");
		return FAILED;
	}

	uint8_t connectToken[ConnectTokenBytes];
	matcher->GetConnectToken(connectToken);

	Godot::print("received connect token from matcher");

	double time = OS::get_ticks_msec();

	config.protocolId = ProtocolId;

	client = new yojimbo::Client(yojimbo::GetDefaultAllocator(), yojimbo::Address("0.0.0.0"), config, adapter, time);

	Address serverAddress("127.0.0.1", port);

	client->Connect(client_id, connectToken);

	if (client->IsDisconnected()) {
		return FAILED;
	}

	char addressString[256];
	client->GetAddress().ToString(addressString, sizeof(addressString));
	Godot::print(String("client address is ") + String(addressString));

	return OK;
}

int NetworkedMultiplayerYojimbo::create_server(int port, int max_clients, int in_bandwidth, int out_bandwidth) {
	if (initialize_yojimbo() != OK) {
		return FAILED;
	}

	if (server != nullptr) {
		return FAILED;
	}

	config.protocolId = ProtocolId;

	uint8_t privateKey[yojimbo::KeyBytes] = { 0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea,
		0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4,
		0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
		0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };

	double time = OS::get_ticks_msec();

	server = new yojimbo::Server(yojimbo::GetDefaultAllocator(), privateKey, yojimbo::Address("127.0.0.1", port), config, adapter, time);

	Godot::print("starting server (secure)");
	server->Start(max_clients);

	return OK;
}

void NetworkedMultiplayerYojimbo::set_bind_ip(String ip) {
}

int NetworkedMultiplayerYojimbo::get_connection_status() const {
	if (client == nullptr) {
		return FAILED;
	}
	if (client->IsDisconnected()) {
		return FAILED;
	}
	if (server->IsClientConnected(client->GetClientIndex())) {
		return OK;
	}
	return FAILED;
}

int NetworkedMultiplayerYojimbo::get_packet_peer() const {
	return OK;
}

int NetworkedMultiplayerYojimbo::get_unique_id() const {
	return OK;
}

void NetworkedMultiplayerYojimbo::poll() {
	if (server != nullptr) {
		server->SendPackets();
		server->ReceivePackets();
		server->AdvanceTime(OS::get_ticks_msec());
	}
	if (client != nullptr) {
		client->SendPackets();
		client->ReceivePackets();
		client->AdvanceTime(OS::get_ticks_msec());
	}

	if (server != nullptr) {
		if (!server) {
			return;
		}
		if (!client) {
			return;
		}
		if (server->GetNumConnectedClients() == 0) {
			return;
		}

		Message *message = server->ReceiveMessage(client->GetClientIndex(), RELIABLE_ORDERED_CHANNEL);
		if (!message) {
			return;
		}
		yojimbo_assert(message->GetId() == (uint16_t)numMessagesReceivedFromClient);
		switch (message->GetType()) {
			case TEST_BLOCK_MESSAGE: {
				TestBlockMessage *blockMessage = (TestBlockMessage *)message;
				yojimbo_assert(blockMessage->sequence == uint16_t(numMessagesReceivedFromClient));
				const int blockSize = blockMessage->GetBlockSize();
				const uint8_t *blockData = blockMessage->GetBlockData();
				yojimbo_assert(blockData);
				printf("server received message %d\n", uint16_t(numMessagesReceivedFromClient));
				PoolByteArray block;
				for (size_t i = 0; i < blockMessage->GetBlockSize(); i++) {
					block.append(blockData[i]);
				}
				server->ReleaseMessage(client->GetClientIndex(), message);
				numMessagesReceivedFromClient++;
				PoolByteArray buffer = PoolByteArray(block);
				if (!client) {
					return;
				}
				if (!server) {
					return;
				}
				if (!server->IsClientConnected(client->GetClientIndex())) {
					return;
				}
				TestBlockMessage *message = (TestBlockMessage *)server->CreateMessage(client->GetClientIndex(), TEST_BLOCK_MESSAGE);
				if (message) {
					message->sequence = (uint16_t)numMessagesSentToClient;
					const int32_t block_size = buffer.size() % MaxBlockSize;
					uint8_t *block_data = server->AllocateBlock(client->GetClientIndex(), block_size);
					if (!block_data) {
						server->ReleaseMessage(client->GetClientIndex(), message);
						return;
					}
					for (int j = 0; j < block_size; ++j) {
						block_data[j] = buffer[j] & 0xff;
					}
					server->AttachBlockToMessage(client->GetClientIndex(), message, block_data, block_size);
					server->SendMessage(client->GetClientIndex(), RELIABLE_ORDERED_CHANNEL, message);
					numMessagesSentToClient++;
					Godot::print("Sent packet from server");
					return;
				}
				return;
				break;
			}
		}
	}
}

void NetworkedMultiplayerYojimbo::set_target_peer(int id) {
}

int NetworkedMultiplayerYojimbo::get_available_packet_count() const {
	return OK;
}

PoolByteArray NetworkedMultiplayerYojimbo::get_packet() {
	if (!client->IsConnected()) {
		return PoolByteArray();
	}
	Message *message = client->ReceiveMessage(RELIABLE_ORDERED_CHANNEL);
	if (!message) {
		return PoolByteArray();
	}
	yojimbo_assert(message->GetId() == (uint16_t)numMessagesReceivedFromServer);

	if (message->GetType() != TEST_BLOCK_MESSAGE) {
		return PoolByteArray();
	}
	TestBlockMessage *blockMessage = (TestBlockMessage *)message;
	yojimbo_assert(blockMessage->sequence == uint16_t(numMessagesReceivedFromServer));
	const int blockSize = blockMessage->GetBlockSize();
	const uint8_t *blockData = blockMessage->GetBlockData();
	yojimbo_assert(blockData);
	PoolByteArray block;
	for (size_t i = 0; i < blockMessage->GetBlockSize(); i++) {
		block.append(blockData[i] & 0xFF);
	}
	printf("client received message size %d\n", block.size());
	client->ReleaseMessage(message);
	numMessagesReceivedFromServer++;
	return block;
}

int NetworkedMultiplayerYojimbo::get_packet_error() const {
	return OK;
}

Variant NetworkedMultiplayerYojimbo::get_var() {
	const String base64 = Marshalls::raw_to_base64(get_packet());
	const Variant variant = Marshalls::base64_to_variant(base64);
	return variant;
}

int NetworkedMultiplayerYojimbo::put_packet(PoolByteArray buffer) {
	if (buffer.size() == 0) {
		return FAILED;
	}
	if (!client) {
		return FAILED;
	}
	if (!client->IsConnected()) {
		return FAILED;
	}
	if (!client->CanSendMessage(RELIABLE_ORDERED_CHANNEL)) {
		return FAILED;
	}
	// TODO Switch between RELIABLE_ORDERED and UNRELIABLE_UNORDERED  
	TestBlockMessage *message = (TestBlockMessage *)client->CreateMessage(TEST_BLOCK_MESSAGE);
	if (message) {
		message->sequence = (uint16_t)numMessagesSentToServer;
		const int32_t block_size = int32_t(buffer.size()) % MaxBlockSize;
		uint8_t *block_data = client->AllocateBlock(block_size);
		if (!block_data) {
			client->ReleaseMessage(message);
			return FAILED;
		}
		for (int j = 0; j < block_size; ++j) {
			block_data[j] = buffer[j] & 0xFF;
		}
		client->AttachBlockToMessage(message, block_data, block_size);
		client->SendMessage(RELIABLE_ORDERED_CHANNEL, message);
		numMessagesSentToServer++;
		Godot::print("Sent packet from client");
		return OK;
	}
	return FAILED;
}

// From NetworkedMultiplayerENet::_gen_unique_id()
uint32_t NetworkedMultiplayerYojimbo::gen_unique_id_() const {

	uint32_t hash = 0;

	while (hash == 0 || hash == 1) {
		char buffer[1024];
		snprintf(buffer, 1024, "%d|%lld", hash, OS::get_ticks_msec());
		hash = String(buffer).to_int();
		snprintf(buffer, 1024, "%d|%lld", hash, OS::get_unix_time());
		hash = String(buffer).to_int();
		snprintf(buffer, 1024, "%d|%d", hash, OS::get_user_data_dir().hash());
		hash = String(buffer).to_int();
		/*
		hash = hash_djb2_one_32(
		(uint32_t)OS::get_singleton()->get_unique_id().hash64(), hash );
		*/
		snprintf(buffer, 1024, "%d|%lld", hash, (uint64_t)this);
		hash = String(buffer).to_int();
		snprintf(buffer, 1024, "%d|%lld", hash, ((uint64_t)&hash));
		hash = String(buffer).to_int();

		hash = hash & 0x7FFFFFFF; // make it compatible with unsigned, since negative id is used for exclusion
	}

	return hash;
}

int NetworkedMultiplayerYojimbo::put_var(const Variant var) {
	PoolByteArray raw = Marshalls::base64_to_raw(Marshalls::variant_to_base64(var));
	return put_packet(raw);
}

void NetworkedMultiplayerYojimbo::_register_methods() {
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

	// Custom
	register_method("set_log_level", &NetworkedMultiplayerYojimbo::set_log_level);

	//register_signal<NetworkedMultiplayerYojimbo>("connection_failed");
	//register_signal<NetworkedMultiplayerYojimbo>("connection_succeeded");
	//register_signal<NetworkedMultiplayerYojimbo>("server_disconnected");
	//register_signal<NetworkedMultiplayerYojimbo>("peer_connected");
	//register_signal<NetworkedMultiplayerYojimbo>("server_disconnected");
}
