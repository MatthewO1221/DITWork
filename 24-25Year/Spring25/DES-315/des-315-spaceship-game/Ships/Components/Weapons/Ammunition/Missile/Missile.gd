class_name Missile
extends Ammunition

@export_category("Missile Values")
@export var armingTime : float = 3.0
@export var burnTime : float = 10.0
@export var fizzleTime : float = 5.0
@export var detectionRange : float = 6000.0
@export var missileSpeed : float = 1000.0



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
			
			
	RaycastSetup()
	
	
func RaycastSetup() -> void:
	
	var rayCount : int = raycasts.size()
	var coneAngle : float = deg_to_rad(90) # Full cone in radians
	var halfAngle : float = coneAngle / 2

	for i in range(rayCount):
		var ray : RayCast2D = raycasts[i]
		if ray == null:
			continue
		
		# Calculate normalized spread from -1 to 1
		var t : float = float(i) / float(rayCount - 1)  # goes from 0 to 1
		var spreadFactor : float = lerp(-1.0, 1.0, t)   # -1 (left), 0 (center), 1 (right)

		# Calculate angle offset from forward (-Y), rotated in global space
		var angle : float = spreadFactor * halfAngle
		
		# Direction vector pointing in the angle (relative to -Y being forward)
		var direction := Vector2.UP.rotated(angle)  # UP is -Y

		# Set cast_to relative to missile
		ray.target_position = direction * detectionRange
		

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
			Search(delta)
		MissileStates.Homing:
			burnTime -= delta
			if burnTime <= 0.0:
				ChangeState(MissileStates.Fizzling)
				return
			Home()
		MissileStates.Fizzling:
			fizzleTime -= delta
			
			if fizzleTime <= 0.0:
				queue_free()

	
	


func Search(delta : float) -> void:
	for raycast in raycasts:
		raycast.force_raycast_update()
		
		if raycast.is_colliding():
			target = raycast.get_collider() as Node2D
			ChangeState(MissileStates.Homing)
			return
			
	rotation_degrees += 90 * delta

func Home() -> void:
	if !is_instance_valid(target):
		ChangeState(MissileStates.Searching)
		return
		
	var direction = (target.global_position - global_position).normalized()
	rotation = direction.angle() + deg_to_rad(90)  # +90° because -Y is forward
	
	apply_central_force(-transform.y * missileSpeed)

	
	
func Destroy(other: Node2D) -> void:
	EffectSpawner.SpawnExplosion(global_position, Vector2(1,1), 10.0)
	queue_free()

func Die() -> void:
	EffectSpawner.SpawnExplosion(global_position, Vector2(1,1), 10.0)
	queue_free()

func ChangeState(newState : MissileStates) -> void:
	state = newState
