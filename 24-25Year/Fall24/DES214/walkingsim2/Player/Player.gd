class_name Player
extends CharacterBody3D


@export var Speed := 5.0
@export var JumpStrength := 4.5
@export var RotateSpeed := 5.0


func _physics_process(delta: float) -> void:
	# Add the gravity.
	if not is_on_floor():
		velocity += get_gravity() * delta

	# Handle jump.
	if Input.is_action_just_pressed("Jump") and is_on_floor():
		velocity.y = JumpStrength

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var input_dir := Input.get_vector("Left", "Right", "Forward", "Backward")
	var direction := (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	if direction:
		velocity.x = direction.x * Speed
		velocity.z = direction.z * Speed
	else:
		velocity.x = move_toward(velocity.x, 0, Speed)
		velocity.z = move_toward(velocity.z, 0, Speed)
		
	if Input.is_action_pressed("RotateLeft") and !Input.is_action_pressed("RotateRight"):
		rotation.y += RotateSpeed * delta
	elif Input.is_action_pressed("RotateRight") and !Input.is_action_pressed("RotateLeft"):
		rotation.y -= RotateSpeed * delta

	move_and_slide()
