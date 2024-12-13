class_name FallingRock
extends RigidBody3D


var area: Area3D

var falling := false

var fallSpeed := 30.0
var fallHeight := -2.23
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	area = get_node(get_meta("Area")) as Area3D
	area.area_entered.connect(collided)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	CheckHeight()
	if falling:
		position.y -= fallSpeed * delta

func CheckHeight() -> void:
	if position.y <= fallHeight:
		falling = false
func collided(otherArea: Area3D) -> void:
	if otherArea.get_parent().is_in_group("Player"):
		var player = otherArea.get_parent() as Player
		player.LogEvent()
		falling = true
