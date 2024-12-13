class_name PopupSpawner
extends Node3D


@export var text: String
@export var lifetime: float

@export var zoneName: String

var popup = load("res://Popups/Popup.tscn") as PackedScene

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass



func SpawnPopup() -> void:
	var newPopup = popup.instantiate()
	newPopup.Init(text, lifetime)
	add_child(newPopup)


func _on_area_3d_area_entered(area: Area3D) -> void:
	if area.get_parent().is_in_group("Player"):
		var player = area.get_parent() as Player
		player.ChangeZone(zoneName)
		SpawnPopup()
