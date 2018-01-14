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

#include <Animation.hpp>
#include <ArrayMesh.hpp>
#include <NetworkedMultiplayerPeer.hpp>
#include <Node.hpp>
#include <Ref.hpp>
#include <Reference.hpp>
#include <String.hpp>
#include <core/Godot.hpp>
#include <core/GodotGlobal.hpp>

using namespace godot;

class NetworkedMultiplayerYojimbo : public GodotScript<NetworkedMultiplayerPeer> {
private:
    GODOT_CLASS(NetworkedMultiplayerYojimbo);

public:
    NetworkedMultiplayerYojimbo() {}
    ~NetworkedMultiplayerYojimbo() { /* close_connection(); */ }

    Array get_extensions() const;

    void close_connection ();
    int create_client (String ip, int port, int in_bandwidth=0, int out_bandwidth = 0);
    int create_server (int port, int max_clients=32, int in_bandwidth=0, int out_bandwidth = 0);
    void set_bind_ip (String ip);

    // NetworkedMultiplayerPeer
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

    /*
    Signals
    connection_failed ( )
    Emitted when a connection attempt fails.

    connection_succeeded ( )
    Emitted when a connection attempt succeeds.

    peer_connected ( int id )
    Emitted by the server when a client connects.

    peer_disconnected ( int id )
    Emitted by the server when a client disconnects.

    server_disconnected ( )
    Emitted by clients when the server disconnects.

    Member Variables
    bool refuse_new_connections - If true this NetworkedMultiplayerPeer refuses new connections. Default value: false.
    TransferMode transfer_mode - The manner in which to send packets to the target_peer. See enum TransferMode.
    Numeric Constants
    TARGET_PEER_BROADCAST = 0 — Packets are sent to the server and then redistributed to other peers.
    TARGET_PEER_SERVER = 1 — Packets are sent to the server alone.
    Enums
    enum TransferMode

    TRANSFER_MODE_UNRELIABLE = 0 — Packets are sent via unordered UDP packets.
    TRANSFER_MODE_UNRELIABLE_ORDERED = 1 — Packets are sent via ordered UDP packets.
    TRANSFER_MODE_RELIABLE = 2 — Packets are sent via TCP packets.
    enum ConnectionStatus

    CONNECTION_DISCONNECTED = 0 — The ongoing connection disconnected.
    CONNECTION_CONNECTING = 1 — A connection attempt is ongoing.
    CONNECTION_CONNECTED = 2 — The connection attempt succeeded.
    */

    /*
    Member Variables
    bool allow_object_decoding
    */
    static void _register_methods();
};