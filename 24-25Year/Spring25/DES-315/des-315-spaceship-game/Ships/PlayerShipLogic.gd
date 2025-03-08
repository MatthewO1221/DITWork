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


var curLinearVel : float = 0.0
var curLinearAccel : float = 0.0

var curAngularVel : float = 0.0
var curAngularAccel : float = 0.0

var curLinearJerk : float = 0.0

var curAngularJerk : float = 0.0



func _physics_process(delta: float) -> void:
	var forwardVector = -transform.y
	
	CalculateNewLinearAcceleration(delta)
	
	CalculateNewLinearVelocity(delta)
	
	CalculateNewAngularAcceleration(delta)
	
	CalculateNewAngularVelocity(delta)

	velocity = forwardVector * curLinearVel
	
	rotation_degrees += curAngularVel * delta
	
	move_and_slide()
	
	print(curLinearJerk)
	
	
func CalculateNewLinearAcceleration(delta: float) -> void:
	if curLinearAccel > 0.0 and curLinearJerk < 0.0:
		curLinearAccel += curLinearJerk * delta * linearDecelerationScalar
	elif curLinearAccel < 0.0 and curLinearJerk > 0.0:
		curLinearAccel += curLinearJerk * delta * linearDecelerationScalar
	else:
		curLinearAccel += curLinearJerk * delta
		
	if curLinearAccel >= maxLinearAcceleration:
		curLinearAccel = maxLinearAcceleration
	
	if curLinearAccel <= -maxLinearAcceleration:
		curLinearAccel = -maxLinearAcceleration
		
	if curLinearJerk == 0.0:
		curLinearAccel -= curLinearAccel * linearDrag * delta
		
		
func CalculateNewAngularAcceleration(delta: float) -> void:
	if curAngularAccel > 0.0 and curAngularJerk < 0.0:
		curAngularAccel += curAngularJerk * delta * angularDecelerationScalar
	elif curAngularAccel < 0.0 and curAngularJerk > 0.0:
		curAngularAccel += curAngularJerk * delta * angularDecelerationScalar
	else:
		curAngularAccel += curAngularJerk * delta
		
	if curAngularAccel >= maxAngularAcceleration:
		curAngularAccel = maxAngularAcceleration
	
	if curAngularAccel <= -maxAngularAcceleration:
		curAngularAccel = -maxAngularAcceleration
		
	if curAngularJerk == 0.0:
		curAngularAccel -= curAngularAccel * angularDrag * delta

func CalculateNewLinearVelocity(delta: float) -> void:
	curLinearVel += curLinearAccel * delta
	
	if curLinearVel >= maxLinearVelocity:
		curLinearVel = maxLinearVelocity
	if curLinearVel <= -maxLinearVelocity:
		curLinearVel = -maxLinearVelocity
		
	if curLinearJerk == 0.0:
		curLinearVel -= curLinearVel * linearDrag * delta
		
func CalculateNewAngularVelocity(delta: float) -> void:
	curAngularVel += curAngularAccel * delta
	
	if curAngularVel >= maxAngularVelocity:
		curAngularVel = maxAngularVelocity
	if curAngularVel <= -maxAngularVelocity:
		curAngularVel = -maxAngularVelocity

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
