class_name RotateAction
extends InfluencerAction






func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
newEndRot: float,
easingMethod: CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, lastsFor, delayedFor, repeats, affects, Rotate, easingMethod, parentAction)
	
	
	
	curve.SetFinish(newEndRot)


func Start() -> void:
	curve.SetStart(entity.global_rotation_degrees)

func Rotate() -> void:
	entity.global_rotation_degrees = curve.GetValue(timePassed, duration)
