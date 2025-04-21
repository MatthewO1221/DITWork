class_name PlayerShip
extends RigidBody2D




@export_category("Velocity Values")
@export var maxLinearVelocity : float
@export var maxAngularVelocity : float

@export_category("Acceleration Values")
@export var maxLinearAcceleration : float
@export var maxAngularAcceleration : float

@export_category("Jerk Values")
@export var linearJerk : float
@export var angularJerk : float

@export_category("Decceleration Values")
@export var linearDeccelerationScalar : float
@export var angularDeccelerationScalar : float

@export_category("Damping Values")
@export var linearDampingScalar : float
@export var angularDampingScalar : float

@export_category("Damaged Values")
@export var maximumManeuveringRatio : float
@export var minimumManeuveringRatio : float
@export var maxBulletTimeScale : float
@export var minBulletTimeScale : float




var curLinearVelocity : float = 0.0
var curLinearAcceleration : float = 0.0
var curLinearJerk : float = 0.0

var curAngularVelocity : float = 0.0
var curAngularAcceleration : float = 0.0
var curAngularJerk : float = 0.0


var curBulletTimeScale : float = 1.0
var curManeuveringRatio : float = 1.0


var chaingun : ChaingunComponent = null
var missileLauncher : MissileLauncher = null

func _ready() -> void:
	chaingun = get_node(get_meta("Chaingun")) as ChaingunComponent
	missileLauncher = get_node(get_meta("MissileLauncher")) as MissileLauncher
	
func _physics_process(delta: float) -> void:
	CalculateNewLinearAcceleration(delta)
	CalculateNewLinearVelocity(delta)
	
	
	
	linear_velocity *= 1.0 - (linearDampingScalar * delta)
	
	linear_velocity += -transform.y * curLinearVelocity * delta
	
	print(linear_velocity)
	
	linear_velocity = linear_velocity.limit_length(maxLinearVelocity)
	
	
	CalculateNewAngularAcceleration(delta)
	CalculateNewAngularVelocity(delta)
	
	angular_velocity *=  1.0 - (angularDampingScalar * delta)
	
	angular_velocity += curAngularVelocity * delta
	
	if angular_velocity > 0.0:
		angular_velocity = minf(angular_velocity, maxAngularVelocity)
	elif angular_velocity < 0.0:
		angular_velocity = maxf(angular_velocity, -maxAngularVelocity)
		
		
		
		
func CalculateNewLinearAcceleration(delta : float) -> void:
	curLinearAcceleration *= 1.0 - ( linearDampingScalar * delta)
	
	if curLinearJerk == 0.0:
		return
		
		
	if curLinearJerk * curLinearAcceleration > 0.0:
		curLinearAcceleration += curLinearJerk * delta * curManeuveringRatio
	else:
		curLinearAcceleration += curLinearJerk * delta * curManeuveringRatio * linearDeccelerationScalar
		
		
	if curLinearAcceleration > 0.0:
		curLinearAcceleration = minf(curLinearAcceleration, maxLinearAcceleration)
	else:
		curLinearAcceleration = maxf(curLinearAcceleration, -maxLinearAcceleration)
		
	
func CalculateNewLinearVelocity(delta : float) -> void:
	curLinearVelocity *= 1.0 - (linearDampingScalar * delta)
	
	if curLinearAcceleration == 0.0:
		return
		
	if curLinearVelocity * curLinearAcceleration > 0.0:
		curLinearVelocity += curLinearAcceleration * delta * curManeuveringRatio
	else:
		curLinearVelocity += curLinearAcceleration * delta * curManeuveringRatio * linearDeccelerationScalar
		
	if curLinearVelocity > 0.0:
		curLinearVelocity = minf(curLinearVelocity, maxLinearVelocity)
	else:
		curLinearVelocity = maxf(curLinearVelocity, -maxLinearVelocity)
	
func CalculateNewAngularAcceleration(delta : float) -> void:
	curAngularAcceleration *= 1 - (angularDampingScalar * delta)
	
	if curAngularJerk == 0.0:
		return
		
	if curAngularAcceleration * curAngularJerk > 0.0:
		curAngularAcceleration += curAngularJerk * delta * curManeuveringRatio
	else:
		curAngularAcceleration += curAngularJerk * delta * curManeuveringRatio * angularDeccelerationScalar
		
	if curAngularAcceleration > 0.0:
		curAngularAcceleration = minf(curAngularAcceleration, maxAngularAcceleration)
	else:
		curAngularAcceleration = maxf(curAngularAcceleration, -maxAngularAcceleration)
	
func CalculateNewAngularVelocity(delta : float) -> void:
	curAngularVelocity *= 1.0 - (angularDampingScalar * delta)
	
	if curAngularAcceleration == 0.0:
		return
		
	if curAngularVelocity * curAngularAcceleration > 0.0:
		curAngularVelocity += curAngularAcceleration * delta * curManeuveringRatio
	else:
		curAngularVelocity += curAngularAcceleration * delta * curManeuveringRatio * angularDeccelerationScalar
		
	if curAngularVelocity > 0.0:
		curAngularVelocity = minf(curAngularVelocity, maxAngularVelocity)
	else:
		curAngularVelocity = maxf(curAngularVelocity, -maxAngularVelocity)
		
	
	
	
	
#@export var Explosion : PackedScene
#
#@export_category("Physics Values")
#@export var maxLinearVelocity : float
#@export var maxLinearAcceleration : float
#@export var linearJerk : float
#@export var maxAngularVelocity : float
#@export var maxAngularAcceleration : float
#@export var angularJerk : float
#@export var linearDecelerationScalar : float
#@export var angularDecelerationScalar : float
#@export var linearDrag : float
#@export var angularDrag : float
#
#
#
#@export var minBulletTime := 0.5
#@export var maxBulletTime := 0.1
#
#var curLinearVel : float = 0.0
#var curLinearAccel : float = 0.0
#
#var curAngularVel : float = 0.0
#var curAngularAccel : float = 0.0
#
#var curLinearJerk : float = 0.0
#
#var curAngularJerk : float = 0.0
#
#var maneuveringRatio := 1.0
#
#var chaingun : ChaingunComponent = null
#var missileLauncher : MissileLauncher = null
#
#var smokeEmitter : GPUParticles2D = null
#
#var health : Hitbox = null
#
#var dead : bool = false
#
#@export var deathTimer : float = 5.0
#var curDeathTimer : float = 0.0
#
#func _ready() -> void:
	#chaingun = get_node(get_meta("Chaingun")) as ChaingunComponent
	#missileLauncher = get_node(get_meta("MissileLauncher")) as MissileLauncher
	#
	#smokeEmitter = get_node(get_meta("Particles")) as GPUParticles2D
	#health = get_node(get_meta("Health")) as Hitbox
	#
	#health.HealthDepleted.connect(Die)
#func _physics_process(delta: float) -> void:
	#
	#if curDeathTimer >= deathTimer:
		#curDeathTimer = 0.0
		#dead = false
		#health.currentHealth = health.maxHealth
		#global_position = Vector2.ZERO
	#
	#if dead:
		#curDeathTimer += delta
		#Engine.time_scale = 1
		#return
	#
	#var forwardVector = -transform.y
	#
	#CalculateNewLinearAcceleration(delta)
	#
	#CalculateNewLinearVelocity(delta)
	#
	#CalculateNewAngularAcceleration(delta)
	#
	#CalculateNewAngularVelocity(delta)
#
	#velocity = forwardVector * curLinearVel
	#
	#
	#rotation_degrees += curAngularVel * delta
	#
	#move_and_slide()
	#
	##print(curLinearJerk)
	#
	#UpdateEmitter()
	#
	#UpdateManeuveringRatio()
	#
	#if float(health.currentHealth) / float(health.maxHealth) <= 0.25 and health.currentHealth > 0.0:
		#BulletTime()
	#else:
		#Engine.time_scale = 1.0
		#
	#
	#
#func CalculateNewLinearAcceleration(delta: float) -> void:
	#if curLinearAccel > 0.0 and curLinearJerk < 0.0:
		#curLinearAccel += curLinearJerk * delta * linearDecelerationScalar * maneuveringRatio
	#elif curLinearAccel < 0.0 and curLinearJerk > 0.0:
		#curLinearAccel += curLinearJerk * delta * linearDecelerationScalar * maneuveringRatio
	#else:
		#curLinearAccel += curLinearJerk * delta * maneuveringRatio
		#
	#if curLinearAccel >= maxLinearAcceleration * maneuveringRatio:
		#curLinearAccel = maxLinearAcceleration * maneuveringRatio
	#
	#if curLinearAccel <= -maxLinearAcceleration * maneuveringRatio:
		#curLinearAccel = -maxLinearAcceleration * maneuveringRatio
		#
	#if curLinearJerk == 0.0:
		#curLinearAccel -= curLinearAccel * linearDrag * delta
		#
		#
#func CalculateNewAngularAcceleration(delta: float) -> void:
	#if curAngularAccel > 0.0 and curAngularJerk < 0.0:
		#curAngularAccel += curAngularJerk * delta * angularDecelerationScalar * maneuveringRatio
	#elif curAngularAccel < 0.0 and curAngularJerk > 0.0:
		#curAngularAccel += curAngularJerk * delta * angularDecelerationScalar * maneuveringRatio
	#else:
		#curAngularAccel += curAngularJerk * delta * maneuveringRatio
		#
	#if curAngularAccel >= maxAngularAcceleration * maneuveringRatio:
		#curAngularAccel = maxAngularAcceleration * maneuveringRatio
	#
	#if curAngularAccel <= -maxAngularAcceleration * maneuveringRatio:
		#curAngularAccel = -maxAngularAcceleration * maneuveringRatio
		#
	#if curAngularJerk == 0.0:
		#curAngularAccel -= curAngularAccel * angularDrag * delta
#
#func CalculateNewLinearVelocity(delta: float) -> void:
	#curLinearVel += curLinearAccel * delta
	#
	#if curLinearVel >= maxLinearVelocity * maneuveringRatio:
		#curLinearVel = maxLinearVelocity * maneuveringRatio
	#if curLinearVel <= -maxLinearVelocity * maneuveringRatio:
		#curLinearVel = -maxLinearVelocity * maneuveringRatio
		#
	#if curLinearJerk == 0.0:
		#curLinearVel -= curLinearVel * linearDrag * delta
		#
#func CalculateNewAngularVelocity(delta: float) -> void:
	#curAngularVel += curAngularAccel * delta
	#
	#if curAngularVel >= maxAngularVelocity * maneuveringRatio:
		#curAngularVel = maxAngularVelocity * maneuveringRatio
	#if curAngularVel <= -maxAngularVelocity * maneuveringRatio:
		#curAngularVel = -maxAngularVelocity * maneuveringRatio
#
	#if curAngularJerk == 0.0:
		#curAngularVel -= curAngularVel * angularDrag * delta
#
#func _unhandled_input(event: InputEvent) -> void:
	#if event.is_action_pressed("Move Forward"):
		#curLinearJerk = linearJerk
	#if event.is_action_pressed("Move Backward"):
		#curLinearJerk = -linearJerk
	#if event.is_action_pressed("Rotate Clockwise"):
		#curAngularJerk = angularJerk
	#if event.is_action_pressed("Rotate Counter-Clockwise"):
		#curAngularJerk = -angularJerk
		#
	#if event.is_action_released("Move Forward") or event.is_action_released("Move Backward"):
		#curLinearJerk = 0.0
	#if event.is_action_released("Rotate Clockwise") or event.is_action_released("Rotate Counter-Clockwise"):
		#curAngularJerk = 0.0
		#
		#
	#if event.is_action_pressed("FireChaingun"):
		#chaingun.StartFiring()
	#if event.is_action_released("FireChaingun"):
		#chaingun.StopFiring()
	#
	#if event.is_action_pressed("FireMissiles"):
		#missileLauncher.StartLaunching()
	#
	#if event.is_action_pressed("Debug_TakeDamage"):
		#health.TakeDamage(10)
#
#func UpdateEmitter() -> void:
	#var healthPercentage : float = float(health.currentHealth) / float(health.maxHealth)
	#
	#if healthPercentage > 0.5:
		#smokeEmitter.emitting = false
		#return
		#
	#smokeEmitter.emitting = true
	#
	#smokeEmitter.amount_ratio = 0.1 / healthPercentage
#
#func UpdateManeuveringRatio() -> void:
	#var healthPercentage := float(health.currentHealth) / float(health.maxHealth)
	#
	#if healthPercentage > 0.5:
		#maneuveringRatio = 1.0
		#return
		#
	#maneuveringRatio = clampf(healthPercentage * 2.0, 0.5, 1.0)
#
#func BulletTime() -> void:
	#var newTimeScale := maxBulletTime + ((minBulletTime - maxBulletTime) * ((float(health.currentHealth) / float(health.maxHealth)) / 0.25))
#
#
	#if health.currentHealth <= 0.0:
		#Engine.time_scale = 1
		#return
#
	#Engine.time_scale = newTimeScale
#
#func Die() -> void:
	#dead = true
	#var explosion = Explosion.instantiate()
	#
	#get_tree().current_scene.add_child(explosion)
	#
	#explosion.global_position = global_position
	#
	#explosion.Explode()
	#
	#PopupText.SetText("Ship Destroyed")

func UpdateManeuveringRatio(newHealth : float) -> void:
	if newHealth / $Hitbox.maxHealth > 0.5:
		curManeuveringRatio = maximumManeuveringRatio
		return
		
	var damageRatio = (newHealth / $Hitbox.maxHealth) / 0.5
	
	curManeuveringRatio = minimumManeuveringRatio + ((maximumManeuveringRatio - minimumManeuveringRatio) * damageRatio)
	
func UpdateBulletTime(newHealth : float) -> void:
	if newHealth / $Hitbox.maxHealth > 0.25:
		curBulletTimeScale = 1
		Engine.time_scale = curBulletTimeScale
		return
		
	var damageRatio = (newHealth / $Hitbox.maxHealth) / 0.25
	
	curBulletTimeScale = minBulletTimeScale + ((maxBulletTimeScale - minBulletTimeScale) * damageRatio)
	
	Engine.time_scale = curBulletTimeScale
	
	
func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("Move Forward"):
		curLinearJerk = linearJerk
	elif event.is_action_released("Move Forward"):
		curLinearJerk = 0
	if event.is_action_pressed("Move Backward"):
		curLinearJerk = -linearJerk
	elif event.is_action_released("Move Backward"):
		curLinearJerk = 0
	if event.is_action_pressed("Rotate Clockwise"):
		curAngularJerk = angularJerk
	elif event.is_action_released("Rotate Clockwise"):
		curAngularJerk = 0
	if event.is_action_pressed("Rotate Counter-Clockwise"):
		curAngularJerk = -angularJerk
	elif event.is_action_released("Rotate Counter-Clockwise"):
		curAngularJerk = 0
		
	if event.is_action_pressed("FireChaingun"):
		chaingun.StartFiring()
	elif event.is_action_released("FireChaingun"):
		chaingun.StopFiring()
		
	if event.is_action_pressed("FireMissiles"):
		missileLauncher.StartLaunching()
