class_name PlayerShip
extends CharacterBody2D


@export_category("Physics Values")
@export var maxLinearVelocity : float
@export var maxLinearAcceleration : float
@export var linearJerk : float
@export var maxAngularVelocity : float
@export var maxAngularAcceleration : float
@export var angularJerk : float
@export var linearDecelerationScalar : float
@export var angularDecelerationScalar : float
@export var linearDrag : float
@export var angularDrag : float



@export var minBulletTime := 0.5
@export var maxBulletTime := 0.1

var curLinearVel : float = 0.0
var curLinearAccel : float = 0.0

var curAngularVel : float = 0.0
var curAngularAccel : float = 0.0

var curLinearJerk : float = 0.0

var curAngularJerk : float = 0.0

var maneuveringRatio := 1.0

var chaingun : Chaingun = null
var missileLauncher : MissileLauncher = null

var smokeEmitter : GPUParticles2D = null

var health : Health = null

func _ready() -> void:
	chaingun = get_node(get_meta("Chaingun")) as Chaingun
	missileLauncher = get_node(get_meta("MissileLauncher")) as MissileLauncher
	
	smokeEmitter = get_node(get_meta("Particles")) as GPUParticles2D
	health = get_node(get_meta("Health")) as Health
func _physics_process(delta: float) -> void:
	var forwardVector = -transform.y
	
	CalculateNewLinearAcceleration(delta)
	
	CalculateNewLinearVelocity(delta)
	
	CalculateNewAngularAcceleration(delta)
	
	CalculateNewAngularVelocity(delta)

	velocity = forwardVector * curLinearVel
	
	rotation_degrees += curAngularVel * delta
	
	move_and_slide()
	
	#print(curLinearJerk)
	
	UpdateEmitter()
	
	UpdateManeuveringRatio()
	
	if float(health.curHealth) / float(health.maxHealth) <= 0.25:
		BulletTime()
	else:
		Engine.time_scale = 1.0
	
func CalculateNewLinearAcceleration(delta: float) -> void:
	if curLinearAccel > 0.0 and curLinearJerk < 0.0:
		curLinearAccel += curLinearJerk * delta * linearDecelerationScalar * maneuveringRatio
	elif curLinearAccel < 0.0 and curLinearJerk > 0.0:
		curLinearAccel += curLinearJerk * delta * linearDecelerationScalar * maneuveringRatio
	else:
		curLinearAccel += curLinearJerk * delta * maneuveringRatio
		
	if curLinearAccel >= maxLinearAcceleration * maneuveringRatio:
		curLinearAccel = maxLinearAcceleration * maneuveringRatio
	
	if curLinearAccel <= -maxLinearAcceleration * maneuveringRatio:
		curLinearAccel = -maxLinearAcceleration * maneuveringRatio
		
	if curLinearJerk == 0.0:
		curLinearAccel -= curLinearAccel * linearDrag * delta
		
		
func CalculateNewAngularAcceleration(delta: float) -> void:
	if curAngularAccel > 0.0 and curAngularJerk < 0.0:
		curAngularAccel += curAngularJerk * delta * angularDecelerationScalar * maneuveringRatio
	elif curAngularAccel < 0.0 and curAngularJerk > 0.0:
		curAngularAccel += curAngularJerk * delta * angularDecelerationScalar * maneuveringRatio
	else:
		curAngularAccel += curAngularJerk * delta * maneuveringRatio
		
	if curAngularAccel >= maxAngularAcceleration * maneuveringRatio:
		curAngularAccel = maxAngularAcceleration * maneuveringRatio
	
	if curAngularAccel <= -maxAngularAcceleration * maneuveringRatio:
		curAngularAccel = -maxAngularAcceleration * maneuveringRatio
		
	if curAngularJerk == 0.0:
		curAngularAccel -= curAngularAccel * angularDrag * delta

func CalculateNewLinearVelocity(delta: float) -> void:
	curLinearVel += curLinearAccel * delta
	
	if curLinearVel >= maxLinearVelocity * maneuveringRatio:
		curLinearVel = maxLinearVelocity * maneuveringRatio
	if curLinearVel <= -maxLinearVelocity * maneuveringRatio:
		curLinearVel = -maxLinearVelocity * maneuveringRatio
		
	if curLinearJerk == 0.0:
		curLinearVel -= curLinearVel * linearDrag * delta
		
func CalculateNewAngularVelocity(delta: float) -> void:
	curAngularVel += curAngularAccel * delta
	
	if curAngularVel >= maxAngularVelocity * maneuveringRatio:
		curAngularVel = maxAngularVelocity * maneuveringRatio
	if curAngularVel <= -maxAngularVelocity * maneuveringRatio:
		curAngularVel = -maxAngularVelocity * maneuveringRatio

	if curAngularJerk == 0.0:
		curAngularVel -= curAngularVel * angularDrag * delta

func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("Move Forward"):
		curLinearJerk = linearJerk
	if event.is_action_pressed("Move Backward"):
		curLinearJerk = -linearJerk
	if event.is_action_pressed("Rotate Clockwise"):
		curAngularJerk = angularJerk
	if event.is_action_pressed("Rotate Counter-Clockwise"):
		curAngularJerk = -angularJerk
		
	if event.is_action_released("Move Forward") or event.is_action_released("Move Backward"):
		curLinearJerk = 0.0
	if event.is_action_released("Rotate Clockwise") or event.is_action_released("Rotate Counter-Clockwise"):
		curAngularJerk = 0.0
		
		
	if event.is_action_pressed("FireChaingun"):
		chaingun.StartFiring()
	if event.is_action_released("FireChaingun"):
		chaingun.StopFiring()
	
	if event.is_action_pressed("FireMissiles"):
		missileLauncher.StartLaunching()
	
	if event.is_action_pressed("Debug_TakeDamage"):
		health.Damage(10)

func UpdateEmitter() -> void:
	var healthPercentage : float = float(health.curHealth) / float(health.maxHealth)
	
	if healthPercentage > 0.5:
		smokeEmitter.emitting = false
		return
		
	smokeEmitter.emitting = true
	
	smokeEmitter.amount_ratio = 0.1 / healthPercentage

func UpdateManeuveringRatio() -> void:
	var healthPercentage := float(health.curHealth) / float(health.maxHealth)
	
	if healthPercentage > 0.5:
		maneuveringRatio = 1.0
		return
		
	maneuveringRatio = clampf(healthPercentage * 2.0, 0.5, 1.0)


func BulletTime() -> void:
	var newTimeScale := maxBulletTime + ((minBulletTime - maxBulletTime) * ((float(health.curHealth) / float(health.maxHealth)) / 0.25))

	Engine.time_scale = newTimeScale
