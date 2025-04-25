class_name ArrangeHandAction
extends Action

## The hand container this action affects
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
	#Call parent init
	super(blocksGroups, blocksEverything, groupName, delayedFor, repeats, parentAction)
	handContainer = affects
	children = []
	
	#Get the layout of all the cards
	var layout : Array[Transform2D] = handContainer.HandArrangement(handContainer.hand.size())
	
	#Iterate through every card in hand
	for i in range(handContainer.hand.size()):
		var card = handContainer.hand.keys()[i]
		var target = layout[i]
		
		var targetPos = target.origin
		var targetRot = rad_to_deg(target.get_rotation())
		
		#Have to create a new curve for every action to avoid conflicts, dumb but it works
		var temp := CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
		var newTranslateAction = TranslateAction.new(blocksGroups, blocksEverything, groupName, lastsFor, delayedFor, repeats, card, targetPos, temp, self)
		
		temp = CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
		var newRotateAction = RotateAction.new(blocksGroups, blocksEverything, groupName, lastsFor, delayedFor, repeats, card, targetRot, temp, self)
		
		#A translate and rotate action for every card
		children.push_back(newTranslateAction)
		children.push_back(newRotateAction)
		
##Get this action's type
func GetActionType() -> String:
	return "ArrangeHand"
