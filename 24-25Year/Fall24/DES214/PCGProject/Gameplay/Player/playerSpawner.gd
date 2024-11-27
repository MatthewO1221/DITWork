class_name PlayerSpawner
extends Node2D


@export var player: PackedScene


var playerSave: CharacterController
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	SpawnPlayer()


# Called every frame. 'delta' is the elapsed time since the previous frame.


func SpawnPlayer() -> void:
	
		
	var playerInstance = player.instantiate() as Node2D
	print(playerInstance.name)
	playerInstance.global_position = global_position  # Set spawn position to spawner's position
	get_tree().current_scene.add_child(playerInstance)
	
	playerSave = playerInstance

func RespawnPlayer() -> void:
	playerSave.queue_free()
	SpawnPlayer()
