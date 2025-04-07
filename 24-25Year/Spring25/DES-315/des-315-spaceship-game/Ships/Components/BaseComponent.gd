class_name Component
extends Node2D


var parent : Node2D = null


func _ready() -> void:
	parent = get_parent()
