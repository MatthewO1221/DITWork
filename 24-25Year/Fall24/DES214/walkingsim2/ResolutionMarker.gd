class_name ResolutionMarker
extends Area3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

	

func _on_area_entered(area: Area3D) -> void:
	if area.get_parent().is_in_group("Player"):
		var player = area.get_parent() as Player
		player.ChangeZone("Resolution")
