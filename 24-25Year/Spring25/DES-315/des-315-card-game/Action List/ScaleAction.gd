class_name ScaleAction
extends InfluencerAction


var initialScale: Vector2
var endScale: Vector2


func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
newEndScale: Vector2,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, affects, Scale, parentAction)
	
	endScale = newEndScale

func Start() -> void:
	initialScale = entity.global_scale

func Scale() -> void:
	entity.global_scale = lerp(initialScale, endScale, GetPercentDone())
