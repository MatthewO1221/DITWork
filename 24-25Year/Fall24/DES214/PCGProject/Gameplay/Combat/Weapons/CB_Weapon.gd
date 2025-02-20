class_name WeaponBase
extends Node2D


enum FiringModes
{
	Burst,
	Spread,
	Shotgun
}

@export_category("Weapon Variables")
@export var bullet: PackedScene
@export var reloadTime := 1.0
@export var numBullets := 1
@export var timeBetweenBullets := 0.1
@export var spreadAngleDeg := 15
@export var firingMode: FiringModes

@export_category("Bullet Variables")
@export var bulletDamage := 1
@export var bulletSize := 1.0
@export var bulletSpeed := 1.0
@export var bulletRange := 10.0
@export var bounces := 0


var reloadCountdown: float

signal Firing(shooter: Node, weaponType: FiringModes)
signal Reloaded

func Fire() -> void:
	pass
	
func _ready() -> void:
	reloadCountdown = reloadTime
	
	

func _process(delta: float) -> void:
	reloadCountdown -= delta
	
	if reloadCountdown <= 0.0:
		emit_signal("Reloaded")

func SpawnBullet() -> Node:
	var bulletInstance = bullet.instantiate()
	
	if !bullet:
		assert("Failed to spawn bullet")
	
	get_tree().current_scene.add_child(bulletInstance)
	bulletInstance.bulletDamage = bulletDamage
	bulletInstance.bulletSize = bulletSize
	bulletInstance.bulletSpeed = bulletSpeed
	bulletInstance.bulletRange = bulletRange
	bulletInstance.bounces = bounces

	# Position the bullet at the weapon's position
	bulletInstance.global_position = self.global_position
	bulletInstance.rotation = self.rotation

	# Add the bullet to the scene
	
	
	return bulletInstance


func FireBurst(target: Vector2) -> void:
	emit_signal("Firing", get_parent(), firingMode)
	
	for i in range(numBullets):
		# Create and configure the bullet instance
		
		var bulletInstance = SpawnBullet()
		# Calculate direction towards the player
		var direction = (target - self.global_position).normalized()
		
		# Set bullet velocity or other parameters (damage, speed, etc.)
		bulletInstance.velocity = direction * bulletSpeed  # Ensure bullet has velocity handling
		
		await get_tree().create_timer(timeBetweenBullets).timeout
	
func FireShotgun(target: Vector2) -> void:
	emit_signal("Firing", get_parent(), firingMode)
	
	# Calculate the direction towards the target
	var targetDirection = (target - self.global_position).normalized()
	var targetAngle = targetDirection.angle()
	
	# Define the shotgun spread angle in radians
	var spreadAngle := deg_to_rad(spreadAngleDeg)  # Total spread angle of 15 degrees
	
	

	for i in range(numBullets):
		
		var bulletInstance = SpawnBullet()
		# Calculate the angle for this bullet in the spread
		var randOffset = randf_range(-spreadAngle / 2, spreadAngle / 2)
		
		var angle = targetAngle + randOffset
		
		
		# Set the bullet's velocity in the direction of the calculated angle
		bulletInstance.velocity = Vector2(cos(angle), sin(angle)) * bulletSpeed
		
		
		
		await get_tree().create_timer(timeBetweenBullets).timeout
	
func FireSpread(target: Vector2) -> void:
	emit_signal("Firing", get_parent(), firingMode)
	
	# Calculate the direction towards the target
	var target_direction = (target - self.global_position).normalized()
	
	# Define the spread angle
	var total_spread_angle := deg_to_rad(spreadAngleDeg)  # Total spread angle of 45 degrees
	var start_angle = target_direction.angle() - total_spread_angle / 2

	for i in range(numBullets):
		
		var bulletInstance = SpawnBullet()
		
		# Calculate each bullet's angle in the spread
		var angle = start_angle + (total_spread_angle * i / max(1, numBullets - 1))
		
		
		
		# Set the bullet's velocity in the direction of the calculated angle
		bulletInstance.velocity = Vector2(cos(angle), sin(angle)) * bulletSpeed
		
		
