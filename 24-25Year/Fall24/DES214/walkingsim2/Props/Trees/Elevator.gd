class_name Elevator
extends CSGCylinder3D

var Area: Area3D
@export var risingSpeed := 5.0
@export var stopHeight := 62.0
var shouldRise := false
var player: Player
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Area = get_node(get_meta("Area")) as Area3D
	Area.area_entered.connect(PlayerCollided)
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	ReachedHeight()
	if shouldRise:
		player.rising = true
		position.y += risingSpeed * delta
		player.position.y += risingSpeed * delta

func ReachedHeight() -> void:
	if position.y >= 62.0:
		shouldRise = false
		if player.rising == true:
			player.position.y += 5.0
		player.rising = false
		
		
		
		
	

func PlayerCollided(otherArea: Area3D) -> void:
	if otherArea.get_parent().is_in_group("Player"):
		shouldRise = true
		player = otherArea.get_parent() as Player
