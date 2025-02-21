class_name RotateAction
extends InfluencerAction


var initialRot: float
var endRot: float


func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
newEndRot: float,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, affects, Rotate, parentAction)
	
	endRot = newEndRot


func Start() -> void:
	initialRot = entity.global_rotation_degrees

func Rotate() -> void:
	entity.global_rotation_degrees = lerpf(initialRot, endRot, GetPercentDone())
