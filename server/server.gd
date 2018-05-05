extends Control
var yojimbo 

func _ready():
	yojimbo = NetworkedMultiplayerYojimbo.new()
	yojimbo.set_bind_ip("127.0.0.1")
	yojimbo.create_server(40000, 4, 4000, 4000)
	#get_tree().set_network_peer(yojimbo)

func _physics_process(delta):
	yojimbo.poll()

func _on_Button_pressed():
	pass
