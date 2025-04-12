class_name Missile
extends Ammunition

@export_category("Missile Values")
@export var armingTime : float = 3.0
@export var burnTime : float = 10.0
@export var fizzleTime : float = 5.0

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

var curLinearVelocity : float = 0.0
var curLinearAcceleration : float = 0.0
var curLinearJerk : float = 0.0

var curAngularVelocity : float = 0.0
var curAngularAcceleration : float = 0.0
var curAngularJerk : float = 0.0

enum MissileStates
{
	Arming,
	Searching,
	Homing,
	Fizzling
}

var state : MissileStates = MissileStates.Arming
var target : Node2D

var raycasts : Array[RayCast2D] = []


func _ready() -> void:
	var children = get_children()
	
	for child in children:
		if child is RayCast2D:
			raycasts.push_back(child)

func _physics_process(delta: float) -> void:
	match state:
		MissileStates.Arming:
			armingTime -= delta
			if armingTime <= 0.0:
				ChangeState(MissileStates.Searching)
		MissileStates.Searching:
			burnTime -= delta
			if burnTime <= 0.0:
				ChangeState(MissileStates.Fizzling)
				return
			Search()
			UpdatePhysics(delta)
		MissileStates.Homing:
			burnTime -= delta
			if burnTime <= 0.0:
				ChangeState(MissileStates.Fizzling)
				return
			Home()
			UpdatePhysics(delta)
		MissileStates.Fizzling:
			fizzleTime -= delta
			
			if fizzleTime <= 0.0:
				queue_free()
			UpdatePhysics(delta)

	
	


func Search() -> void:
	for raycast in raycasts:
		raycast.force_raycast_update()
		
		if raycast.is_colliding():
			target = raycast.get_collider() as Node2D
			ChangeState(MissileStates.Homing)
			return
			
	curAngularJerk = angularJerk

func Home() -> void:
	if !is_instance_valid(target):
		ChangeState(MissileStates.Searching)
		return
		
	var targetPos = target.global_position
	
	var curForward = -transform.y.normalized()
	
	var desiredForward = (targetPos - global_position).normalized()
	
	var angleOfChange = rad_to_deg(curForward.angle_to(desiredForward))
	
	var desiredAngularVelocity = sign(angleOfChange) * maxAngularVelocity * (abs(angleOfChange) / PI)
	
	
	

	if curAngularVelocity < desiredAngularVelocity :
		curAngularJerk = angularJerk
	elif curAngularVelocity > desiredAngularVelocity:
		curAngularJerk = -angularJerk
	else:
		curAngularJerk = 0.0
		
		
		
	curLinearJerk = linearJerk



func UpdatePhysics(delta : float) -> void:
	UpdateLinearPhysics(delta)
	UpdateAngularPhysics(delta)
	
	linear_velocity = -transform.y.normalized() * curLinearVelocity
	angular_velocity = curAngularVelocity
	


func UpdateLinearPhysics(delta: float) -> void:
	UpdateLinearAcceleration(delta)
	UpdateLinearVelocity(delta)
	
func UpdateAngularPhysics(delta: float) -> void:
	UpdateAngularAcceleration(delta)
	UpdateAngularVelocity(delta)
		
func UpdateLinearAcceleration(delta: float) -> void:
	if curLinearAcceleration > 0.0 and curLinearJerk < 0.0:
		curLinearAcceleration += curLinearJerk * delta * linearDecelerationScalar
	elif curLinearAcceleration < 0.0 and curLinearJerk > 0.0:
		curLinearAcceleration += curLinearJerk * delta * linearDecelerationScalar
	else:
		curLinearAcceleration += curLinearJerk * delta
		
	if curLinearAcceleration >= maxLinearAcceleration:
		curLinearAcceleration = maxLinearAcceleration
	
	if curLinearAcceleration <= -maxLinearAcceleration:
		curLinearAcceleration = -maxLinearAcceleration
		
	if curLinearJerk == 0.0:
		curLinearAcceleration -= curLinearAcceleration * linearDrag * delta

func UpdateLinearVelocity(delta: float) -> void:
	curLinearVelocity += curLinearAcceleration * delta
	
	if curLinearVelocity >= maxLinearVelocity:
		curLinearVelocity = maxLinearVelocity
	if curLinearVelocity <= -maxLinearVelocity:
		curLinearVelocity = -maxLinearVelocity
		
	if curLinearJerk == 0.0:
		curLinearVelocity -= curLinearVelocity * linearDrag * delta


func UpdateAngularAcceleration(delta: float) -> void:
	if curAngularAcceleration > 0.0 and curAngularJerk < 0.0:
		curAngularAcceleration += curAngularJerk * delta * angularDecelerationScalar
	elif curAngularAcceleration < 0.0 and curAngularJerk > 0.0:
		curAngularAcceleration += curAngularJerk * delta * angularDecelerationScalar
	else:
		curAngularAcceleration += curAngularJerk * delta
		
	if curAngularAcceleration >= maxAngularAcceleration:
		curAngularAcceleration = maxAngularAcceleration
	
	if curAngularAcceleration <= -maxAngularAcceleration:
		curAngularAcceleration = -maxAngularAcceleration
		
	if curAngularJerk == 0.0:
		curAngularAcceleration -= curAngularAcceleration * angularDrag * delta


func UpdateAngularVelocity(delta: float) -> void:
	curAngularVelocity += curAngularAcceleration * delta
	
	if curAngularVelocity >= maxAngularVelocity:
		curAngularVelocity = maxAngularVelocity
	if curAngularVelocity <= -maxAngularVelocity:
		curAngularVelocity = -maxAngularVelocity

	if curAngularJerk == 0.0:
		curAngularVelocity -= curAngularVelocity * angularDrag * delta

	
func Destroy(other: Node2D) -> void:
	queue_free()


func ChangeState(newState : MissileStates) -> void:
	state = newState
