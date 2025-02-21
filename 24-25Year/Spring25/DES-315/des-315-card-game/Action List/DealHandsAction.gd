class_name DealHandsAction
extends BenignAction


var handContainers : Array[HandContainer]

func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Array[HandContainer],
numCardsToDeal: int,
deck: DeckContainer,
secondsBetweenCards: float,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats)
	handContainers = affects
	children = []
	
	
	var cardNum = 0
	
	for num in range(numCardsToDeal):
		for hand in handContainers:
			var card := deck.RemoveTopCard()
			
			hand.AddCard(card)
			
			var arrangeAction := ArrangeHandAction.new(blocksGroups, blocksEverything, groupNum, lastsFor, cardNum * secondsBetweenCards, repeats, hand, self)
	
			
			children.push_back(arrangeAction)
			
			cardNum += 1
