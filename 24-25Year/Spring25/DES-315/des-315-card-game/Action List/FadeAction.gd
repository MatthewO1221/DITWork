class_name FadeAction
extends UIInfluencerAction






func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Control,
newEndA: float,
easingMethod: CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, lastsFor, delayedFor, repeats, affects, Fade, easingMethod, parentAction)
	
	
	
	curve.SetFinish(newEndA)


func Start() -> void:
	curve.SetStart(entity.modulate.a)

func Fade() -> void:
	entity.modulate.a = curve.GetValue(timePassed, duration)
