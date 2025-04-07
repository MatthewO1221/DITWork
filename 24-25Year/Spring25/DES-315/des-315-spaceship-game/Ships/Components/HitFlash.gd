class_name HitFlash
extends Node

@export var sprite: Sprite2D
@export var flashDuration: float = 0.1

var flashColor := Color.WHITE


func Flash() -> void:
	sprite.modulate = Color.BLACK
	
	await get_tree().create_timer(flashDuration).timeout
	
	sprite.modulate = Color.WHITE
