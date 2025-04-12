class_name Asteroid
extends RigidBody2D

@export var Explosion : PackedScene


func Die() -> void:
	
	var explosion = Explosion.instantiate()
	
	get_tree().current_scene.add_child(explosion)
	
	explosion.global_position = global_position
	
	explosion.Explode()
	
	PopupText.SetText("Asteroid Destroyed")
	
	queue_free()
