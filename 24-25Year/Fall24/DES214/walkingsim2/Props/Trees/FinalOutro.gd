class_name OutroTrigger
extends Area3D

var outroScene = load("res://Popups/Outro.tscn") as PackedScene
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass



func _on_area_entered(area: Area3D) -> void:
	if area.get_parent().is_in_group("Player"):
		SpawnOutro()
		
func SpawnOutro() -> void:
	var outro = outroScene.instantiate()
	add_child(outro)
