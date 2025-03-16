class_name DealHandsAction
extends BenignAction


var handContainers : Array[HandContainer]

func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Array[HandContainer],
numCardsToDeal: int,
deck: DeckContainer,
secondsBetweenCards: float,
easingMethod: CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, delayedFor, repeats)
	handContainers = affects
	children = []
	
	
	var cardNum = 0
	
	for num in range(numCardsToDeal):
		for hand in handContainers:
			var card := deck.RemoveTopCard()
			
			hand.AddCard(card)
			
			var temp := CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
			var arrangeAction := ArrangeHandAction.new(blocksGroups, blocksEverything, groupName, lastsFor, cardNum * secondsBetweenCards, repeats, hand, temp, self)
	
			
			children.push_back(arrangeAction)
			
			cardNum += 1

func GetActionType() -> String:
	return "DealHands"
