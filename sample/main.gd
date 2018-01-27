extends Control

export (NativeScript) var native_yojimbo
var yojimbo 

func _ready():
	yojimbo = native_yojimbo.new()
	yojimbo.create_server(40000, 2, 0, 0)

#	get_tree().set_network_peer(yojimbo)

func _physics_process(delta):
	yojimbo.poll()

func _on_Button_pressed():
	yojimbo.create_client("127.0.0.1", 40000, 0, 0)
	yojimbo.set_log_level(2)