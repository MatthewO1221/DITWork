class_name DiscardContainer
extends CardContainer

## The offset applied to each card from the one below it, gives discard a 3d look
@export var offset: Vector2

## The cards in the discard pile
var cards: Array[CardBase]


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta * actionSpeed)

## Offsets all cards, works similarly to [method DeckContainer.OffsetCards]
func OffsetCards() -> void:
	
	for i in cards.size():
		var card = cards[cards.size() - (i + 1)]
		
		var newCardPosition = global_position + (offset * (cards.size() - (i + 1)))
		card.z_index = i
		
		var arrangeCurve := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
		
		var moveAction := TranslateAction.new(false, false, "ArrangeDiscard", 0.1, 1.5, false, cards[i], newCardPosition, arrangeCurve)
		
		actionList.PushBack(moveAction)

## Make all cards face down
func HideCards() -> void:
	for card in cards:
		card.ShowBack()

## Make all cards face up
func ShowCards() -> void:
	for card in cards:
		card.ShowFace()

## Adds a card to the discard
func AddCard(card : CardBase) -> void:
	cards.push_back(card)
	OffsetCards()

## Adds a card to the discard and uses actions to move it
func GrabCard(card: CardBase) -> void:
	
	AddCard(card)
	
	var grabCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
	var moveAction := TranslateAction.new(false, false, "DiscardCard", 1.0, 0.0, false, card, global_position, grabCurve)
	
	actionList.PushBack(moveAction)
	
	var grabCurve2 := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
	var rotateAction := RotateAction.new(true, false, "DiscardCard", 1.0, 0.0, false, card, 0.0, grabCurve2)
	
	actionList.PushBack(rotateAction)
	
	OffsetCards()
	
## Grabs all cards from given [TrickContainer]
func GrabCards(trick: TrickContainer) -> void:
	
	for card in trick.cards.values():
		GrabCard(card)

## Returns top card in discard
func GetTopCard() -> CardBase:
	return cards.back()

## Removes top card and returns it
func RemoveTopCard() -> CardBase:
	var card = GetTopCard()
	cards.pop_back()
	return card

## Shuffles discard, does not use shuffle action
func Shuffle() -> void:
	cards.shuffle()
	OffsetCards()

## Clear out discard
func Reset() -> void:
	for card in cards:
		card.queue_free()
		
	cards.clear()
