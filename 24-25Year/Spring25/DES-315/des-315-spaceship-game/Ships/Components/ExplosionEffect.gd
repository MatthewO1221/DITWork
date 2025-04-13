class_name ExplosionEffect
extends Node2D


@export var randomStrength : float = 30.0
@export var shakeFade : float = 5.0

@export var bullet : PackedScene
@export var bulletAmount : int = 12
@export var bulletSpeed : float = 2500

var rng := RandomNumberGenerator.new()

var shakeStrength : float = 0.0

signal AnimationFinished

var camera : Camera2D = null

func _ready() -> void:
	camera = get_tree().get_first_node_in_group("Camera")


func Explode() -> void:
	$Explosion.play("Explosions")
	
	$Explosion.animation_finished.connect(ExplosionFinished)
	
	ApplyShake()
	
	EmitBullets()
	
func EmitBullets() -> void:
	for i in bulletAmount:
		var angle : float = (TAU / bulletAmount) * i
		
		var direction : Vector2 = Vector2.RIGHT.rotated(angle)
		
		
		var curBullet = bullet.instantiate()
		
		get_tree().current_scene.add_child(curBullet)
		
		curBullet.global_position = global_position
		
		curBullet.linear_velocity = direction * bulletSpeed
		
		curBullet.rotation = direction.angle() + deg_to_rad(90)
	
func _process(delta: float) -> void:
	
	if shakeStrength > 0:
		shakeStrength = lerpf(shakeStrength, 0, shakeFade * delta)
		
		camera.offset = RandomOffset()
	
func ExplosionFinished() -> void:
	AnimationFinished.emit()
	queue_free()


func ApplyShake() -> void:
	shakeStrength = randomStrength
	
	
func RandomOffset() -> Vector2:
	return Vector2(rng.randf_range(-shakeStrength, shakeStrength), rng.randf_range(-shakeStrength, shakeStrength))
