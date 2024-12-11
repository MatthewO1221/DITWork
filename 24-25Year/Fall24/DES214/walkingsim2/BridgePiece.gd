class_name BridgePiece
extends RigidBody3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var area: Area3D
	
	area = get_node(get_meta("Area")) as Area3D
	
	area.area_entered.connect(Collide)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func Collide(otherArea: Area3D) -> void:
	if otherArea.get_parent().is_in_group("Player"):
		gravity_scale = 2.0
