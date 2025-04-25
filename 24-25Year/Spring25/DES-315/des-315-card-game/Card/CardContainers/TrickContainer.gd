class_name TrickContainer
extends CardContainer

## The cards in this container, stored as the hand the card came from and the card itself
var cards : Dictionary[HandContainer, CardBase]

## The horizontal space between each card
@export var spacing := 100.0

## @deprecated
@export var height := 50.0

## The [DiscardContainer] that cards from this trick should go to
@export var discard : DiscardContainer


## Timer for how long cards should stay in the trick
var scoreTimer := Timer.new()

## Amount of time cards should stay in the container before being discarded
@export var scoreTime := 5.0

## Whether cards are currently being scored
var scoring : bool = false

## Reference to game controller
var gameController : GameBoard = null

## Sent when scoring is finished
signal DoneScoring

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	gameController = get_tree().get_first_node_in_group("GameController")
	
	add_child(scoreTimer)
	
	# Make sure timer is affected by engine time, not necessary
	scoreTimer.process_callback = Timer.TIMER_PROCESS_PHYSICS
	
	# When timer runs out call score
	scoreTimer.timeout.connect(Score)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	if !actionList.IsEmpty():
		actionList.UpdateAllActions(delta)
		
		
		
		
## Scores the cards currently in the trick
func Score() -> void:
	var winners = GetWinningCard()
	gameController.AddScore(winners)
	scoring = false
	discard.GrabCards(self)
	cards.clear()
	DoneScoring.emit()

## Similar to [HandContainer] returns array of [Transform2D]s that represent
## where cards should go
func CardArrangement(numCards : int) -> Array[Transform2D]:
	var layout : Array[Transform2D] = []
	
	if numCards == 0:
		return layout
	
	var width : float = (numCards - 1) * spacing
	var center : float  = width / 2
	
	
	var horAxis := global_transform.x.normalized()
	
	var vertAxis := global_transform.y.normalized()
	
	for i in range(numCards):
		var xPos = i * spacing - center
		
		var horOffset := i * spacing - center
		
		var normalized: float = 0.0
		if width > 0.0:
			normalized = abs(horOffset) / width
		else:
			normalized = 0.0
		
		var vertOffset := 0.0 #-height * (1.0 - pow(normalized, 2))
		
		
		var cardPos := global_position + (horOffset * horAxis)
		
		cardPos = cardPos + (vertOffset * vertAxis)
		
		
		
		var newTransform := Transform2D(0.0, cardPos)
		
		layout.push_back(newTransform)
	
	return layout

## Get position that a new card should go to
func GetDealPosition() -> Transform2D:
	var newLayout = CardArrangement(cards.size() + 1)
	
	return newLayout.back()

## Adds the given card from the given hand
func AddCard(hand: HandContainer, card : CardBase) -> void:
	cards[hand] = card
	
## Erases a card from trick
func RemoveCard(card : CardBase) -> void:
	cards.erase(card)

## Grabs a random card from each [HandContainer] in given array
func GrabCards(hands: Array[HandContainer]) -> void:
	
	# Get a random card from each hand
	for hand in hands:
		var card = hand.GetRandom()
		#print(card.valueMap[card.value] + card.suitMap[card.suit])
		hand.RemoveCard(card)
		AddCard(hand, card)
		
	var layouts := CardArrangement(cards.size())
	
	# Use actions to move each card to the trick
	for i in cards.size():
		var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_SPRING, Tween.EaseType.EASE_OUT)
		
		var moveAction := TranslateAction.new(false, false, "GrabCards", 1.0, 0.0, false, cards[cards.keys()[i]], layouts[i].origin, easingMethod)
		
		actionList.PushBack(moveAction)
		
		if i != 0:
		
			var easingMethod2 := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN_OUT)
		
			var flipAction := FlipCardAction.new(false, false, "GrabCards", 0.25, 0.0, false, cards[cards.keys()[i]], easingMethod2)
		
			actionList.PushBack(flipAction)
		
		
		
		
		var easingMethod3 := CustomCurve.new(Tween.TransitionType.TRANS_SPRING, Tween.EaseType.EASE_OUT)
		
		var rotateAction := RotateAction.new(true, false, "GrabCards", 1.0, 0.0, false, cards[cards.keys()[i]], rad_to_deg(layouts[i].get_rotation()), easingMethod3)
		
		actionList.PushBack(rotateAction)
		
		
		var easingMethod4 := CustomCurve.new(Tween.TransitionType.TRANS_SPRING, Tween.EaseType.EASE_IN_OUT)
		
		var arrangeAction := ArrangeHandAction.new(false, false, "ArrangeHand", 2.0, 0.0, false, cards.keys()[i], easingMethod4)
		
		cards.keys()[i].actionList.PushBack(arrangeAction)
	
	# Start the score timer
	scoreTimer.start(scoreTime)

## Adds a single card from the given hand
func GrabCard(hand: HandContainer, card: CardBase) -> void:
	hand.RemoveCard(card)
	AddCard(hand, card)
	
	
	
## Returns the winning card(s) and the hand(s) it/they came from
func GetWinningCard() -> Dictionary[HandContainer, CardBase]:
	
	# If the automator is active record the cards played
	if Automator.curState != Automator.AutomationStates.Inactive:
		Telemeter.CardsPlayed(cards.values())
	
	
	var highestValue : int = 0
	
	# Get the highest value card
	for card in cards.values():
		if card.value > highestValue:
			highestValue = card.value
	
	var winningCards : Dictionary[HandContainer, CardBase]
	
	# Add each card with highest value to winning cards dict
	for key in cards.keys():
		if cards[key].value == highestValue:
			winningCards[key] = cards[key]
			
	return winningCards

## Clear the trick
func Reset() -> void:
	for card in cards.values():
		card.queue_free()
		
	cards.clear()
