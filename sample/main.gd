extends Control

var native_yojimbo = preload("res://addons/com_chibifire_networking_yojimbo/com_chibifire_networking_yojimbo.gdns")
var yojimbo 

func _ready():	
	yojimbo = native_yojimbo.new()
	yojimbo.create_client("127.0.0.1", 4000, 0, 0)

func _on_Button_pressed():
	pass