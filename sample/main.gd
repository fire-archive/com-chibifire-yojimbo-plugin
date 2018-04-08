extends Control
var yojimbo 

func _ready():
	yojimbo = NetworkedMultiplayerYojimbo.new()
	yojimbo.create_server(40000, 2, 4000, 4000)
	get_tree().set_network_peer(yojimbo)

	var timer = Timer.new()
	timer.connect("timeout", self, "on_timer_timeout") 
	add_child(timer)
	timer.one_shot = false
	timer.wait_time = 4.0
	timer.start() 

func _physics_process(delta):
	yojimbo.poll()
	
func on_timer_timeout():
	yojimbo.create_client("127.0.0.1", 40000, 4000, 4000)

func _on_Button_pressed():
	var test = [1, 2, 3, 4]
	yojimbo.put_packet(test)
	var packet = yojimbo.get_packet()
	for elem in packet:
		print(elem)
	yojimbo.put_var("Hello")
	print("Var: " + str(yojimbo.get_var()))
#	yojimbo.close_connection()

