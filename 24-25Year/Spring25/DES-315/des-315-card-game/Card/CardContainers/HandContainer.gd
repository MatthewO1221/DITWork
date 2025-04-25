class_name HandContainer
extends CardContainer

## Held cards, stored as card itself and bool for if card is currently being hovered over
var hand : Dictionary[CardBase, bool]

## Whether this is the player's hand
@export var playerHand : bool = false

## The horizontal spacing between cards
@export var spacing := 100.0

## The minimum and maximum angle of cards, for curve affect
@export var angle := 30.0

## How much higher the center card should be from the ones on the edges
@export var height := 50.0

## The maximum random horizontal offset added to each card
@export var randomHorOffsetMax : float

## The maximum random vertical offset added to each card
@export var randomVertOffsetMax : float

## The minimum and maximum random rotational offset added to each card
@export var randomRotOffsetMinMax : float


@export_category("CardHovering")
## Whether cards in this hand are manually playable
@export var manuallyPlayable : bool = false
## The scale that hovered cards should reach
@export var hoverSize : float = 2.5
## The height that hovered cards should reach
@export var hoverHeight : float = 50.0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta * actionSpeed)

## Returns an array of [Transform2D]s which represent the position and rotation of each card in
## the hand for the given number of cards
func HandArrangement(numCards : int) -> Array[Transform2D]:
	
	# Return array
	var layout : Array[Transform2D] = []
	
	# If the number of cards is 0 just return
	if numCards == 0:
		return layout
	
	# The total width of the hand
	var width : float = (numCards - 1) * spacing
	
	# The center of the hand
	var center : float  = width / 2
	
	# The horizontal axis of this hand
	var horAxis := transform.x.normalized()
	
	# The vertical axis of this hand
	var vertAxis := transform.y.normalized()
	
	# For every card
	for i in range(numCards):
		
		# X position of the card
		var xPos = i * spacing - center
		
		var horOffset := i * spacing - center
		
		var normalized: float = 0.0
		if width > 0.0:
			normalized = abs(horOffset) / width
		else:
			normalized = 0.0
		
		# The vertical offset of the card
		var vertOffset := -height * (1.0 - pow(normalized, 2))
		
		# Creates horizontal part of final card position
		var cardPos := global_position + (horOffset * horAxis)
		
		# Adds vertical component to final card position
		cardPos = cardPos + (vertOffset * vertAxis)
		
		# Get the angle
		var curAngle = -angle/2
		
		if numCards > 1:
			curAngle += angle * i / (numCards - 1)
			
		# Get the random translation of the card
		var randTranslation : Vector2 = Vector2(randf_range(0, randomHorOffsetMax), randf_range(0, randomVertOffsetMax))
		
		# Get the random rotation of the card
		var randRotation : float = randf_range(-randomRotOffsetMinMax, randomRotOffsetMinMax)
		
		# Create a new transform from all this data
		var newTransform := Transform2D(deg_to_rad(curAngle + rotation_degrees + randRotation), cardPos + randTranslation)
		
		layout.push_back(newTransform)
	
	return layout


## Get the position of the given card in the hand
func GetCardHandPos(card: CardBase) -> Transform2D:
	var cardIndex := 0
	
	for cards in hand.keys():
		if cards == card:
			break
		cardIndex += 1
		
	var layouts = HandArrangement(hand.keys().size())
	
	return layouts[cardIndex]

## Get the position that a new card would be dealt to
func GetDealPosition() -> Transform2D:
	var newLayout = HandArrangement(hand.size() + 1)
	
	return newLayout.back()

## Add a new card to the hand
func AddCard(card : CardBase) -> void:
	# Add the card, unhovered
	hand[card] = false
	
	var cardIndex = hand.keys().size() - 1
	
	card.z_index = cardIndex
	
	if playerHand:
		card.ShowFace()
	
	# Connect signals
	card.mouseEntered.connect(CardHovered)
	card.mouseExited.connect(CardUnhovered)
	card.clicked.connect(CardClicked)

## Remove a card from the hand
func RemoveCard(card : CardBase) -> void:
	hand.erase(card)
	card.mouseEntered.disconnect(CardHovered)
	card.mouseExited.disconnect(CardUnhovered)
	card.clicked.disconnect(CardClicked)

## Get a random card from the hand
func GetRandom() -> CardBase:
	return hand.keys().pick_random()

## When the card is hovered
func CardHovered(card: CardBase) -> void:
	
	if hand[card] == true:
		return
		
	#var cardIndex = hand.keys().find(card)
	#
	#var nextCard : CardBase = null
	#
	#if cardIndex + 1 < hand.keys().size():
		#nextCard = hand.keys()[cardIndex + 1]
	#
	#if nextCard != null and hand[nextCard] == true:
		#return
	
	hand[card] = true
	
	# Get the cards y axis
	var cardY = -card.transform.y.normalized()
	
	var easingMethod = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	# The position of the card when hovered
	var newCardPos = GetCardHandPos(card).get_origin() + (cardY * hoverHeight)
	
	var translateAction := TranslateAction.new(false, false, "CardHover" + card.name, 0.5, 0.0, false, card, newCardPos, easingMethod)
	
	var easingMethod2 = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	
	var scaleAction := ScaleAction.new(true, false, "CardHover" + card.name, 0.5, 0.0, false, card, Vector2(hoverSize, hoverSize), easingMethod2)
	
	actionList.PushBack(translateAction)
	actionList.PushBack(scaleAction)
	
## When the card is unhovered
func CardUnhovered(card: CardBase) -> void:
	
	if hand[card] == false:
		return
		
	hand[card] = false
	
	var cardY = -card.transform.y.normalized()
	
	var easingMethod = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var newCardPos = GetCardHandPos(card).get_origin()
	
	var translateAction := TranslateAction.new(false, false, "CardHover" + card.name, 0.5, 0.0, false, card, newCardPos, easingMethod)
	
	var easingMethod2 = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	
	var scaleAction := ScaleAction.new(true, false, "CardHover" + card.name, 0.5, 0.0, false, card, Vector2(2.0, 2.0), easingMethod2)
	
	actionList.PushBack(translateAction)
	actionList.PushBack(scaleAction)

## When a card is clicked
func CardClicked(card: CardBase) -> void:
	if !manuallyPlayable:
		return
	
	var easingMethod2 = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	
	var scaleAction := ScaleAction.new(true, false, "CardHover" + card.name, 0.5, 0.0, false, card, Vector2(2.0, 2.0), easingMethod2)
		
		
	actionList.PushBack(scaleAction)
	
	
	var gameBoard = get_tree().get_first_node_in_group("GameController") as GameBoard
	
	gameBoard.PlayCards(card)
	
## Clears the hand
func Reset() -> void:
	for card in hand.keys():
		card.queue_free()
		
	hand.clear()
