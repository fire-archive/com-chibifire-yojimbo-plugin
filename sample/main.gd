extends Control

export (NativeScript) var native_yojimbo
var yojimbo 

func _ready():
	yojimbo = native_yojimbo.new()
#   Doesn't do anything
#	yojimbo.set_log_level(1)
	yojimbo.create_server(40000, 2, 0, 0)
	get_tree().set_network_peer(yojimbo)

	var timer = Timer.new()
	timer.connect("timeout", self, "_on_timer_timeout") 
	add_child(timer)
	timer.wait_time = 4.0
	timer.start() 

func _on_timer_timeout():
   yojimbo.create_client("127.0.0.1", 2, 0, 0)

func _physics_process(delta):
	yojimbo.poll()

func _on_Button_pressed():
	yojimbo.put_packet([])
	print(yojimbo.get_packet().size())
#	yojimbo.close_connection()