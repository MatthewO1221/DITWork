class_name LevelResetPortal
extends Node2D


@export var Generator: MapGenerator

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var area = get_node(get_meta("Collider")) as Area2D
	
	area.area_entered.connect(OnCollision)
	

func OnCollision(otherArea: Area2D) -> void:
	
	Generator.reset()
