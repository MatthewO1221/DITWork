class_name MissileLauncher
extends Weapon


@export_category("Launcher Variables")
@export var cooldown : float
@export var missileCount : int
@export var launchTimer : float
@export var launchVelocity : float
@export var projectile : PackedScene


var curCooldown : float

var curLaunchTimer : float

var launching : bool = false

var missilesLaunched : int = 0

func _ready() -> void:
	curCooldown = cooldown
	curLaunchTimer = launchTimer
	
func _process(delta: float) -> void:
	curCooldown -= delta

	if launching:
		curLaunchTimer -= delta
	
	if curLaunchTimer <= 0.0:
		curLaunchTimer = launchTimer
		Launch()
		
		if missilesLaunched >= missileCount:
			StopLaunching()

func CanLaunch() -> bool:
	return curCooldown <= 0.0

func StartLaunching() -> void:
	if CanLaunch():
		launching = true
		curCooldown = cooldown
	
func StopLaunching() -> void:
	launching = false
	curLaunchTimer = launchTimer
	missilesLaunched = 0
	
func SpawnMissile() -> Node2D:
	var missile = projectile.instantiate()
	
	
	
	get_tree().current_scene.add_child(missile)
	
	var ammo := missile as Ammunition
	
	var data : Dictionary[String, Variant] = {"Damage" = damage, "knockbackStrength" = knockbackStrength}
	
	ammo.ReceiveData(data)
	
	return missile

func Launch() -> void:
	var horAxis = get_parent().transform.x.normalized()
	
	var rightMissile = SpawnMissile() as Missile
	
	rightMissile.global_position = global_position
	
	rightMissile.linear_velocity = horAxis * launchVelocity
	
	var leftMissile = SpawnMissile() as Missile
	
	leftMissile.global_position = global_position
	
	leftMissile.linear_velocity = -horAxis * launchVelocity
	
	missilesLaunched += 2
