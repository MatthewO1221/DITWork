class_name ShuffleAction
extends InfluencerAction


var cardsToRotate : Array[CardBase]


func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
easingMethod: CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, lastsFor, delayedFor, repeats, affects, Shuffle, easingMethod, parentAction)
	
	
	# Pick two random cards to rotate
	for i in range(2):
		cardsToRotate.push_back(entity.cards.pick_random())
	
	actionFunction = Shuffle
	
	curve.begin = 0.0
	curve.end = 360.0
	
func Shuffle() -> void:
	
	# Rotate cards in opposite directions
	cardsToRotate[0].global_rotation_degrees = curve.GetValue(timePassed, duration)
	cardsToRotate[1].global_rotation_degrees = curve.GetValueFrom(0.0, -360.0, timePassed, duration)

func End() -> void:
	entity.Shuffle()

func GetActionType() -> String:
	return "Shuffle"
