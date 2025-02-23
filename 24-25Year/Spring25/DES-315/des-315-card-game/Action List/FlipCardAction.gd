class_name FlipCardAction
extends BenignAction


var card : CardBase

func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: CardBase,
easingMethod: CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, delayedFor, repeats, parentAction)
	card = affects
	children = []
	
	var cardScale = affects.global_scale
	
	var scaleDownAction = ScaleAction.new(true, false, groupNum, lastsFor, 0.0, repeats, affects, Vector2.ZERO, easingMethod, self)
		
	children.push_back(scaleDownAction)
	
	var flipAction = FlipAction.new(true, false, groupNum, 0.0, repeats, affects, self)
	
	children.push_back(flipAction)
	
	var scaleUpAction = ScaleAction.new(false, false, groupNum, lastsFor, 0.0, repeats, affects, cardScale, easingMethod, self)
		
	children.push_back(scaleUpAction)
