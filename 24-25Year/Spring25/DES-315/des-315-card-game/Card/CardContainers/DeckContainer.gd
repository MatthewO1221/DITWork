class_name DeckContainer
extends CardContainer

## Deck containers area, deprecated
@onready var area = $Area2D

## The offset of each card from the one below it, gives deck a three-dimensional look
@export var offset: Vector2

## The cards in the deck, top card is at back of array
var cards: Array[CardBase]



# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

## Spawns a full deck of cards
func SpawnDeck() -> void:
	# Get all the cards
	cards.append_array(CardFactory.GenerateDeck())
	for card in cards:
		# Move each card to the deck's position and make them face down
		card.global_position = global_position
		card.ShowBack()
	
	OffsetCards()
	Shuffle()
	
	


## Offsets every card using [param offset]
func OffsetCards() -> void:
	
	# Iterate through every card
	for i in cards.size():
		# Start with the top card and go down
		var card = cards[cards.size() - (i + 1)]
		
		# Offset each card
		card.global_position = global_position + (offset * (cards.size() - (i + 1)))
		card.global_rotation_degrees = 0
		card.z_index = i

## Make all cards face down
func HideCards() -> void:
	for card in cards:
		card.ShowBack()

## Adds a card to the deck and then calls [method DeckContainer.OffsetCards] and [method DeckContainer.HideCards]
func AddCard(card : CardBase) -> void:
	cards.push_back(card)
	OffsetCards()
	HideCards()

## Deals 1 card to the given [HandContainer]
func DealCard(container : HandContainer) -> void:
	# Get top card
	var card = cards.pop_back()
	
	container.AddCard(card)
	
	var dealCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
	container.actionList.PushBack(ArrangeHandAction.new(false, false, "DealCard", 1.0, 0.0, false, container, dealCurve))
	
## Returns the top card
func GetTopCard() -> CardBase:
	return cards.back()
	
## Removes top card from deck and returns it
func RemoveTopCard() -> CardBase:
	var card = GetTopCard()
	cards.pop_back()
	return card
	
## Shuffles deck, does not use shuffle action
func Shuffle() -> void:
	cards.shuffle()
	OffsetCards()

## Clears out the deck and removes all cards
func Reset() -> void:
	for card in cards:
		card.queue_free()
		
	cards.clear()
	
## Calls [method DeckContainer.AddCard] on every card in given array
func GrabCards(cards : Array[CardBase]) -> void:
	for card in cards:
		AddCard(card)
