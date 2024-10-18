# EnemyMovement.gd - Script attached to the enemy node in the scene

extends CharacterBody2D

@export var speed: float = 100.0    # Movement speed of the enemy
@export var detectionRange: float = 300.0  # Range within which enemy detects player
@export var moveTowardPlayer: bool = true  # Whether to move toward the player

@export_group("Debug")
@export var player: NodePath  # Player node path to detect

var targetPosition: Vector2  # Position of the detected player
var detected: bool = false    # Whether the player has been detected

# Called when the node enters the scene tree for the first time
func _ready() -> void:
	if player == null:
		print("Player node path is not set.")

# Called every frame
func _process(delta: float) -> void:
	if player != null:
		var playerNode = get_node(player)
		var distanceToPlayer = global_position.distance_to(playerNode.global_position)
		
		# Detect player if within detection range
		if distanceToPlayer <= detectionRange:
			detected = true
			targetPosition = playerNode.global_position
		else:
			detected = false

		# Move toward player if detected
		if detected and moveTowardPlayer:
			move_enemy(delta)

# Moves the enemy toward the player's position
func move_enemy(delta: float) -> void:
	var direction = (targetPosition - global_position).normalized()
	var angleToPlayer = direction.angle()
	rotation = angleToPlayer
	velocity = direction * speed
	move_and_slide()
