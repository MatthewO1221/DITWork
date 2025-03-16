class_name FlipCardAction
extends BenignAction


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
	
	var cardScale = affects.global_scale
	
	var temp1 : CustomCurve = CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
	
	var scaleDownAction = ScaleAction.new(true, false, groupName, lastsFor, 0.0, repeats, affects, Vector2.ZERO, temp1, self)
		
	children.push_back(scaleDownAction)
	
	
	
	var flipAction = FlipAction.new(true, false, groupName, 0.0, repeats, affects, self)
	
	children.push_back(flipAction)
	
	var temp2 : CustomCurve = CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
	
	var scaleUpAction = ScaleAction.new(false, false, groupName, lastsFor, 0.0, repeats, affects, cardScale, temp2, self)
		
	children.push_back(scaleUpAction)

func GetActionType() -> String:
	return "FlipCard"
