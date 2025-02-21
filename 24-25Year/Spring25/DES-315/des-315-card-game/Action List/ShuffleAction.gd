class_name ShuffleAction
extends InfluencerAction


var cardsToRotate : Array[CardBase]


func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, affects, Shuffle, parentAction)
	
	
	
	for i in range(2):
		cardsToRotate.push_back(entity.cards.pick_random())
	
	actionFunction = Shuffle
	
	
func Shuffle() -> void:
	
	cardsToRotate[0].global_rotation_degrees = lerpf(0.0, 360.0, GetPercentDone())
	cardsToRotate[1].global_rotation_degrees = lerpf(0.0, -360.0, GetPercentDone())

func End() -> void:
	entity.Shuffle()
