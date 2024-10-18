extends Node2D

# Bullet parameters
@export var bulletScene: PackedScene
@export var bulletSpeed: float = 500
@export var bulletDamage: int = 10
@export var bulletSize: Vector2 = Vector2(10, 10)
@export var firingRate: float = 0.5
@export var numberOfBullets: int = 5
@export var spreadAngle: float = 30
@export var arcAngle: float = 180
@export var isSpreadShot: bool = true  # Set true for spread shot, false for evenly distributed bullets

# Fire control
var timeSinceLastShot: float = 0

# Reference to player/enemy firing bullets
@export var ownerEntity: Node2D  # This can be any Node2D like Player or Enemy

func _process(delta: float) -> void:
	timeSinceLastShot += delta

	if Input.is_action_just_pressed("fire") and timeSinceLastShot >= firingRate:
		fire_bullets()
		timeSinceLastShot = 0

func fire_bullets() -> void:
	if isSpreadShot:
		fire_in_spread()
	else:
		fire_in_arc()

func fire_in_spread() -> void:
	# Bullets fire in a spread around the owner's forward direction
	var baseRotation = ownerEntity.rotation

	for i in range(numberOfBullets):
		var angleOffset = randf_range(-spreadAngle / 2, spreadAngle / 2)
		fire_single_bullet(baseRotation + deg_to_rad(angleOffset))

func fire_in_arc() -> void:
	# Bullets fire evenly in a chosen arc
	var baseRotation = ownerEntity.rotation
	var startAngle = baseRotation - deg_to_rad(arcAngle / 2)
	var angleStep = deg_to_rad(arcAngle) / (numberOfBullets - 1)

	for i in range(numberOfBullets):
		var bulletAngle = startAngle + angleStep * i
		fire_single_bullet(bulletAngle)

func fire_single_bullet(angle: float) -> void:
	var bullet = bulletScene.instance() as Node2D
	add_child(bullet)
	
	bullet.position = ownerEntity.global_position
	bullet.rotation = angle
	
	var direction = Vector2(cos(angle), sin(angle))
	bullet.set("speed", bulletSpeed)
	bullet.set("damage", bulletDamage)
	bullet.set("size", bulletSize)
	bullet.set("direction", direction)
	
	# Set bullet properties as needed for player or enemy
	bullet.set("owner", ownerEntity)
