class_name ArrangeHandAction
extends BenignAction


var handContainer : HandContainer

func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: HandContainer,
easingMethod: CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, delayedFor, repeats, parentAction)
	handContainer = affects
	children = []
	
	
	var layout : Array[Transform2D] = handContainer.HandArrangement(handContainer.hand.size())
	
	for i in range(handContainer.hand.size()):
		var card = handContainer.hand.keys()[i]
		var target = layout[i]
		
		var targetPos = target.origin
		var targetRot = rad_to_deg(target.get_rotation())
		
		var temp := CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
		var newTranslateAction = TranslateAction.new(blocksGroups, blocksEverything, groupName, lastsFor, delayedFor, repeats, card, targetPos, temp, self)
		
		temp = CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
		var newRotateAction = RotateAction.new(blocksGroups, blocksEverything, groupName, lastsFor, delayedFor, repeats, card, targetRot, temp, self)
		
		
		children.push_back(newTranslateAction)
		children.push_back(newRotateAction)
		

func GetActionType() -> String:
	return "ArrangeHand"
