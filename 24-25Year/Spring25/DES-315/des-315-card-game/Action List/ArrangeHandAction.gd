class_name ArrangeHandAction
extends Action


var handContainer : HandContainer

func _init(newDuration: float, newHandContainer: HandContainer) -> void:
	handContainer = newHandContainer
	children = []
	
	benign = true
	
	var layout : Array[Transform2D] = handContainer.HandArrangement(handContainer.hand.size())
	
	for i in range(handContainer.hand.size()):
		var card = handContainer.hand[i]
		var target = layout[i]
		
		var targetPos = target.origin
		var targetRot = rad_to_deg(target.get_rotation())
		
		var newTranslateAction = TranslateAction.new(newDuration, card, 0.0, false, targetPos)
		newTranslateAction.parent = self
		
		var newRotateAction = RotateAction.new(newDuration, card, 0.0, false, targetRot)
		newRotateAction.parent = self
		
		children.push_back(newTranslateAction)
		children.push_back(newRotateAction)
		
	actionFunction = func() -> void: pass
