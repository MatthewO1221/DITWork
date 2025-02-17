class_name RotateAction
extends Action


var initialRot: float
var endRot: float


func _init(newDuration: float, newEntity: Node2D, newDelay: float, newRepeating: bool, newEndRot: float) -> void:
	super(newDuration, newEntity, newDelay, newRepeating)
	initialRot = entity.global_rotation_degrees
	endRot = newEndRot

	actionFunction = func Rotate() -> void: entity.global_rotation_degrees = lerpf(initialRot, endRot, GetPercentDone())
