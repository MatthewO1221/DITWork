class_name ChaingunComponent
extends Weapon

@export_category("Chaingun Variables")
@export var firingCurve : ControlCurve
@export var projectile : PackedScene
@export var projectileSpeed : float
@export var maxProjectileSpread : float




var timer : float = 0.0

func _ready() -> void:
	super()
	firingCurve.Ready()

func StartFiring() -> void:
	firingCurve.Activate()
	
func _process(delta: float) -> void:
	firingCurve.process(delta)
	
	timer += delta
	
	if !firingCurve.IsActive():
		return
		
	if timer >= 1.0 / firingCurve.GetValue():
		Fire(-parent.global_transform.y)
		timer = 0.0


func StopFiring() -> void:
	firingCurve.Deactivate()


func SpawnAmmo() -> RigidBody2D:
	var bullet = projectile.instantiate()
	
	
	get_tree().current_scene.add_child(bullet)
	
	return bullet as RigidBody2D

func Fire(forwardVector : Vector2) -> void:
	var bullet = SpawnAmmo() as RigidBody2D
	
	forwardVector = forwardVector.rotated(deg_to_rad(randf_range(-maxProjectileSpread * (firingCurve.GetValue() / firingCurve._maxValue.GetValue()), maxProjectileSpread * (firingCurve.GetValue() / firingCurve._maxValue.GetValue()))))
	
	bullet.linear_velocity = forwardVector * projectileSpeed
	
	bullet.global_position = global_position
	
	bullet.global_rotation_degrees = rad_to_deg(forwardVector.angle()) + 90.0
	
	var ammo := bullet as Ammunition
	
	var data : Dictionary[String, Variant] = {"Damage" = damage, "knockbackStrength" = knockbackStrength}
	
	ammo.ReceiveData(data)
	
