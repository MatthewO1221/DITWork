class_name DealHandsAction
extends Action

# The hand containers that are being dealt to
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
	
	# Iterates through number of cards being dealt
	for num in range(numCardsToDeal):
		# For each hand
		for hand in handContainers:
			
			# Get a card from the deck
			var card := deck.RemoveTopCard()
			
			hand.AddCard(card)
			
			# Arrange hand action is used to both deal the card and arrange cards already in the hand
			var temp := CustomCurve.new(easingMethod.curveType, easingMethod.easeType)
			var arrangeAction := ArrangeHandAction.new(blocksGroups, blocksEverything, groupName, lastsFor, cardNum * secondsBetweenCards, repeats, hand, temp, self)
	
			
			children.push_back(arrangeAction)
			
			cardNum += 1

func GetActionType() -> String:
	return "DealHands"
