extends Node2D


var ship : PlayerShip

var smallShip = preload("res://Ships/SmallShip.tscn")
var mediumShip = preload("res://Ships/MediumShip.tscn")
var largeShip = preload("res://Ships/LargeShip.tscn")

func _ready() -> void:
	ship = get_tree().get_first_node_in_group("PlayerShip")




func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("SmallShip"):
		SwitchShip(smallShip)
	if event.is_action_pressed("MediumShip"):
		SwitchShip(mediumShip)
	if event.is_action_pressed("LargeShip"):
		SwitchShip(largeShip)

func SwitchShip(shipType: PackedScene) -> void:
	var newShip = shipType.instantiate()
	newShip.global_position = ship.global_position
	ship.queue_free()
	add_child(newShip)
	ship = newShip
