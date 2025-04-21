extends Camera2D




var playerShip : PlayerShip = null

@export_range(0.0, 1.0, 0.01) var cameraSpeed : float

@export var minZoom : float

@export var maxZoom : float

func _ready() -> void:
	playerShip = get_tree().get_first_node_in_group("PlayerShip")


func _physics_process(delta: float) -> void:
	if !is_instance_valid(playerShip):
		playerShip = get_tree().get_first_node_in_group("PlayerShip")
	
	
	UpdateCameraPos(delta)
	UpdateCameraZoom()


func UpdateCameraPos(delta: float) -> void:
	var targetPos = playerShip.global_position
	
	global_position = global_position.lerp(targetPos, cameraSpeed)
	
	
func UpdateCameraZoom() -> void:
	var curShipSpeed = playerShip.curLinearVelocity
	
	var curSpeedRatio = curShipSpeed / playerShip.maxLinearVelocity
	
	
	var zoomDifference = minZoom - maxZoom
	
	var zoomAmount = minZoom - (zoomDifference * curSpeedRatio)
	
	zoom = Vector2(zoomAmount, zoomAmount)
