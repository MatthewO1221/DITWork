class_name CharacterController
extends CharacterBody2D


@export var speed := 200.0  # Movement speed

var weapon: PlayerWeaponBase


var initSpeed: float
var initHealth: int
var initBullets := 1
var initPos: Vector2

var totalDistance := 0.0

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
	
	totalDistance += velocity.length()
	# Apply movement using the built-in CharacterBody2D methods
	move_and_slide()

	# Update rotation to face mouse position
	var mouse_pos = get_global_mouse_position()
	look_at(mouse_pos)

func _ready() -> void:
	var healthComp = get_node(get_meta("Health")) as HealthComponent
	
	healthComp.deathSignal.connect(OnDeath)
	
	weapon = get_node(get_meta("Weapon")) as PlayerWeaponBase
	
	initSpeed = speed
	
	initHealth = healthComp.maxHealth
	
	initPos = global_position

func OnDeath() -> void:
	var mapGenerator = get_tree().get_first_node_in_group("MapGenerator") as MapGenerator
	
	mapGenerator.reset()
	
func Reset() -> void:
	
	global_position = initPos
	
	weapon.numBullets = initBullets
	
	var healthComp = get_node(get_meta("Health")) as HealthComponent
	
	healthComp.maxHealth = initHealth
	
	healthComp.curHealth = initHealth
	
	speed = initSpeed
	
	TelemetrySystem.LogData("PlayerDistanceTraveled", totalDistance)
	
	totalDistance = 0

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Swap Weapon"):
		match weapon.firingMode:
			weapon.FiringModes.Burst:
				weapon.firingMode = weapon.FiringModes.Spread
				
			weapon.FiringModes.Spread:
				weapon.firingMode = weapon.FiringModes.Shotgun
				weapon.timeBetweenBullets = 0.01
			weapon.FiringModes.Shotgun:
				weapon.firingMode = weapon.FiringModes.Burst
				weapon.timeBetweenBullets = 0.1
				
