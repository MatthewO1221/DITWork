extends CharacterBody3D

@export var sphere_center: Vector3 = Vector3.ZERO
var sphere_radius: float = 5.0
@export var move_speed: float = 0.5

var target_point: Vector3

func _ready():
	# Initialize the firefly with a random target inside the sphere
	var parent = get_parent() as Area3D
	var sphere = parent.get_node(parent.get_meta("Sphere")) as CollisionShape3D
	var shape = sphere.shape as SphereShape3D
	sphere_radius = shape.radius
	target_point = get_random_point_in_sphere()

func _physics_process(delta: float):
	# Move toward the target point
	move_toward_target(delta)

	# Check if close to target and pick a new one
	if position.distance_to(target_point) < 0.5:
		target_point = get_random_point_in_sphere()

func move_toward_target(delta: float):
	# Move smoothly toward the target
	var direction = (target_point - position).normalized()
	velocity = direction * move_speed
	move_and_slide()

func get_random_point_in_sphere() -> Vector3:
	# Generate a random point within the sphere
	while true:
		var random_point = Vector3(
			randf_range(-1.0, 1.0),
			randf_range(-1.0, 1.0),
			randf_range(-1.0, 1.0)
		).normalized() * randf() * sphere_radius
		if random_point.length() <= sphere_radius:
			return sphere_center + random_point
	return Vector3.ZERO
