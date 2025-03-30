class_name Chaingun
extends Weapon

@export var control : ControlCurve

@export var projectileSpeed : float
@export var projectileSpread : float




var timer : float = 0.0

func _ready() -> void:
	control.Ready()

func StartFiring() -> void:
	control.Activate()
	
func _process(delta: float) -> void:
	control.process(delta)
	
	timer += delta
	
	if !control.IsActive():
		return
		
	if timer >= 1.0 / control.GetValue():
		Fire()
		timer = 0.0


func StopFiring() -> void:
	control.Deactivate()


func Fire() -> void:
	var bullet = SpawnAmmo() as RigidBody2D
	
	var ship = get_parent() as CharacterBody2D
	
	var forwardVector = -ship.transform.y.normalized() as Vector2
	
	forwardVector = forwardVector.rotated(deg_to_rad(randf_range(-projectileSpread * control.GetValue(), projectileSpread * control.GetValue())))
	
	bullet.linear_velocity = forwardVector * projectileSpeed
	
	bullet.global_position = ship.global_position
	
	bullet.global_rotation_degrees = rad_to_deg(forwardVector.angle()) + 90.0
	
