class_name GameController
extends Node2D


var cameras: Array



# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func GetCameras() ->  void:
	cameras = get_tree().get_nodes_in_group("Camera")

func SwitchActiveCamera() -> void:
	var camera1 = cameras[0] as Camera2D
	var camera2 = cameras[1] as Camera2D
	
	if camera1.enabled == true:
		camera1.enabled = false
		camera2.enabled = true
	else:
		camera1.enabled = true
		camera2.enabled = false
	

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Swap Camera"):
		SwitchActiveCamera()
