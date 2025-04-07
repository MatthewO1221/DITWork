class_name BeamWeapon
extends Weapon



@export var endPointEntity : Node2D = null



func _ready() -> void:
	assert(endPointEntity != null, "Beam weapon needs end point")
	
	CreateStartPoint()
	CreateEndPoint()
	CreateCollider()


func _process(delta: float) -> void:
	MoveStartPoint(global_position)
	MoveEndPoint(endPointEntity.global_position)
	MoveCollider()

func CreateStartPoint() -> void:
	$Beam.add_point(global_position, 0)


func CreateEndPoint() -> void:
	$Beam.add_point(endPointEntity.global_position, 1)


func MoveStartPoint(pos: Vector2) -> void:
	$Beam.set_point_position(0, pos)

func MoveEndPoint(pos: Vector2) -> void:
	$Beam.set_point_position(1, pos)

func CreateCollider() -> void:
	
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
