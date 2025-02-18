class_name TranslateAction
extends Action


var initialPos: Vector2
var endPos: Vector2


func _init(newDuration: float, newEntity: Node2D, newDelay: float, newRepeating: bool, newEndPos: Vector2) -> void:
	super(newDuration, newEntity, newDelay, newRepeating)
	initialPos = entity.global_position
	endPos = newEndPos

	actionFunction = func Translate() -> void: entity.global_position = entity.global_position.lerp(endPos, GetPercentDone())
