class_name MissileLauncher
extends Weapon


@export var cooldown : float
@export var missileCount : int
@export var launchTimer : float
@export var launchVelocity : float


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

func Launch() -> void:
	var horAxis = get_parent().transform.x.normalized()
	
	var rightMissile = SpawnAmmo() as Missile
	
	rightMissile.global_position = global_position
	
	rightMissile.linear_velocity = horAxis * launchVelocity
	
	var leftMissile = SpawnAmmo() as Missile
	
	leftMissile.global_position = global_position
	
	leftMissile.linear_velocity = -horAxis * launchVelocity
	
	missilesLaunched += 2
