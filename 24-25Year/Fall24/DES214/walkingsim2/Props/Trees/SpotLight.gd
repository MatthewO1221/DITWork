class_name Spotlight
extends CSGCylinder3D

var followPlayer := false
var player: Player
var closeDistance := 200.0
var ogRotation: Vector3

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var area = get_node(get_meta("Area")) as Area3D
	
	area.area_entered.connect(Collided)
	
	ogRotation = rotation
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if followPlayer:
		# Calculate the direction to the player
		var target_direction = (player.global_position - global_position).normalized()
		
		# Manually adjust the spotlight's rotation to align -Y with the target direction
		var transform = Transform3D()
		transform.origin = global_position
		transform.basis.y = -target_direction
		transform.basis.x = global_transform.basis.z.cross(-target_direction).normalized()
		transform.basis.z = transform.basis.y.cross(transform.basis.x).normalized()
		
		# Apply the new transform
		global_transform = transform
		
		if global_position.distance_to(player.global_position) <= closeDistance:
			followPlayer = false
			rotation = ogRotation
			

func Collided(otherArea: Area3D) -> void:
	if otherArea.get_parent().is_in_group("Player"):
		followPlayer = true
		player = otherArea.get_parent() as Player
		player.LogEvent()
		
		
		
