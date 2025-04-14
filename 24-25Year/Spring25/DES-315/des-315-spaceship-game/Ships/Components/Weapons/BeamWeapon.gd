class_name BeamWeapon
extends Weapon



@export var endPointEntity : Node2D = null



func _ready() -> void:
	assert(endPointEntity != null, "Beam weapon needs end point")
	
	CreateStartPoint()
	CreateEndPoint()
	CreateCollider()
	
	$Beam/Hurtbox.damage = damage
	$Beam/Hurtbox.knockbackStrength = knockbackStrength


func _process(delta: float) -> void:
	if !is_instance_valid(endPointEntity):
		queue_free()
		return
	
	MoveStartPoint(to_local(global_position))
	MoveEndPoint(endPointEntity.global_position)
	MoveCollider()

func CreateStartPoint() -> void:
	$Beam.add_point(to_local(global_position), 0)


func CreateEndPoint() -> void:
	var localPos := to_local(endPointEntity.global_position)
	
	$Beam.add_point(localPos, 1)


func MoveStartPoint(pos: Vector2) -> void:
	$Beam.set_point_position(0, pos)

func MoveEndPoint(pos: Vector2) -> void:
	$Beam.set_point_position(1, to_local(pos))

func CreateCollider() -> void:
	
	var points : PackedVector2Array = []
	
	var startPoint := to_global($Beam.points[0]) as Vector2
	var endPoint := to_global($Beam.points[1]) as Vector2
	
	var parallel := (endPoint - startPoint).normalized()
	var perpindicular := Vector2(parallel.y, -parallel.x)
	
	var halfWidth : float = $Beam.width / 2
	
	var point1 := startPoint + halfWidth * perpindicular
	var point2 := endPoint + halfWidth * perpindicular
	var point3 := endPoint - halfWidth * perpindicular
	var point4 := startPoint - halfWidth * perpindicular
	
	points.append($Beam/Hurtbox/Collider.to_local(point1))
	points.append($Beam/Hurtbox/Collider.to_local(point2))
	points.append($Beam/Hurtbox/Collider.to_local(point3))
	points.append($Beam/Hurtbox/Collider.to_local(point4))
	
	$Beam/Hurtbox/Collider.polygon = points
	
func MoveCollider() -> void:
	var points : PackedVector2Array = []
	
	var startPoint := $Beam.points[0] as Vector2
	var endPoint := $Beam.points[1] as Vector2
	
	var parallel := (endPoint - startPoint).normalized()
	var perpindicular := Vector2(parallel.y, -parallel.x)
	
	var halfWidth : float = $Beam.width / 2
	
	var point1 := startPoint + halfWidth * perpindicular
	var point2 := endPoint + halfWidth * perpindicular
	var point3 := endPoint - halfWidth * perpindicular
	var point4 := startPoint - halfWidth * perpindicular
	
	points.append(point1)
	points.append(point2)
	points.append(point3)
	points.append(point4)
	
	$Beam/Hurtbox/Collider.polygon = points
