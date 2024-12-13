extends PathFollow3D

@export var move_speed: float = 2.0
@export var max_distance_from_player: float = 10.0
@export var pause_duration: float = 2.0

var player: Node3D
var is_paused: bool = true


func _ready():
	# Get the player node (assumes the player is named "Player")
	player = get_tree().get_first_node_in_group("Player")
	

func _physics_process(delta: float):
	

	# Check distance to player
	if player and global_position.distance_to(player.global_position) > max_distance_from_player:
		is_paused = true
	else:
		is_paused = false
		
	if is_paused:
		return
	# Move along the path
	progress += move_speed * delta
