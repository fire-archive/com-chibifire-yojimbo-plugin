extends Control

export (NativeScript) var native_yojimbo
var yojimbo 

func _ready():
	yojimbo = native_yojimbo.new()
#	yojimbo.create_server(4000, 2, 0, 0)
#	yojimbo.poll()
	yojimbo.create_client("127.0.0.1", 4000, 0, 0)
#	get_tree().set_network_peer(yojimbo)

func _on_Button_pressed():
	pass