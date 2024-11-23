class_name CharacterController
extends CharacterBody2D


@export var speed := 200.0  # Movement speed

var weapon: PlayerWeaponBase

# This function is for player movement using WASD
func get_input() -> Vector2:
	var input_vector = Vector2.ZERO
	
	# WASD movement
	input_vector.x = Input.get_action_strength("moveRight") - Input.get_action_strength("moveLeft")
	input_vector.y = Input.get_action_strength("moveDown") - Input.get_action_strength("moveUp")
	
	return input_vector.normalized()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(_delta: float) -> void:
	# Get the movement input and apply speed
	var movement_direction = get_input()
	velocity = movement_direction * speed
	
	
	# Apply movement using the built-in CharacterBody2D methods
	move_and_slide()

	# Update rotation to face mouse position
	var mouse_pos = get_global_mouse_position()
	look_at(mouse_pos)

func _ready() -> void:
	var healthComp = get_node(get_meta("Health")) as HealthComponent
	
	healthComp.deathSignal.connect(OnDeath)
	
	weapon = get_node(get_meta("Weapon")) as PlayerWeaponBase

func OnDeath() -> void:
	queue_free()

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Swap Weapon"):
		match weapon.firingMode:
			weapon.FiringModes.Burst:
				weapon.firingMode = weapon.FiringModes.Spread
			weapon.FiringModes.Spread:
				weapon.firingMode = weapon.FiringModes.Shotgun
			weapon.FiringModes.Shotgun:
				weapon.firingMode = weapon.FiringModes.Burst
