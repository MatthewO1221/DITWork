class_name TranslateAction
extends InfluencerAction


var initialPos: Vector2
var endPos: Vector2


func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
newEndPos: Vector2,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, affects, Translate, parentAction)
	
	endPos = newEndPos


func Start() -> void:
	initialPos = entity.global_position

func Translate() -> void: 
	entity.global_position = entity.global_position.lerp(endPos, GetPercentDone())
