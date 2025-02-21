class_name ArrangeHandAction
extends BenignAction


var handContainer : HandContainer

func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: HandContainer,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, parentAction)
	handContainer = affects
	children = []
	
	
	var layout : Array[Transform2D] = handContainer.HandArrangement(handContainer.hand.size())
	
	for i in range(handContainer.hand.size()):
		var card = handContainer.hand[i]
		var target = layout[i]
		
		var targetPos = target.origin
		var targetRot = rad_to_deg(target.get_rotation())
		
		var newTranslateAction = TranslateAction.new(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, card, targetPos, self)
		
		
		var newRotateAction = RotateAction.new(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, card, targetRot, self)
		
		
		children.push_back(newTranslateAction)
		children.push_back(newRotateAction)
		
