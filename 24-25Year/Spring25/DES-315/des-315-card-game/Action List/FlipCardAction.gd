class_name FlipCardAction
extends Action


var card : CardBase

func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: CardBase,
easingMethod: CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, delayedFor, repeats, parentAction)
	card = affects
	children = []
	
	# Get original scale
	var cardScale = affects.global_scale
	
	
	# First scale down the card
	var temp1 : CustomCurve = CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
	
	var scaleDownAction = ScaleAction.new(true, false, groupName, lastsFor, 0.0, repeats, affects, Vector2.ZERO, temp1, self)
		
	children.push_back(scaleDownAction)
	
	
	# Then flip the card
	var flipAction = FlipAction.new(true, false, groupName, 0.0, repeats, affects, self)
	
	children.push_back(flipAction)
	
	# Finally scale the card back up
	var temp2 : CustomCurve = CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
	
	var scaleUpAction = ScaleAction.new(false, false, groupName, lastsFor, 0.0, repeats, affects, cardScale, temp2, self)
		
	children.push_back(scaleUpAction)

func GetActionType() -> String:
	return "FlipCard"
