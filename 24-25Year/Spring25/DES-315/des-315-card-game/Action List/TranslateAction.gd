class_name TranslateAction
extends InfluencerAction


func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
newEndPos: Vector2,
easingMethod: CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, affects, Translate, easingMethod, parentAction)
	
	curve.SetFinish(newEndPos)




func Start() -> void:
	curve.SetStart(entity.global_position)

func Translate() -> void: 
	entity.global_position = curve.GetValue(timePassed, duration)
