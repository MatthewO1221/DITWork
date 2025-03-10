class_name ScaleAction
extends InfluencerAction






func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
newEndScale: Vector2,
easingMethod: CustomCurve, 
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, lastsFor, delayedFor, repeats, affects, Scale, easingMethod, parentAction)
	
	curve.SetFinish(newEndScale)

func Start() -> void:
	curve.SetStart(entity.global_scale)

func Scale() -> void:
	entity.global_scale = curve.GetValue(timePassed, duration)
	
