class_name DiscardContainer
extends Node2D


@export var offset: Vector2


var cards: Array[CardBase]

var actionList := ActionList.new()

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta)

	
func OffsetCards() -> void:
	
	for i in cards.size():
		var card = cards[cards.size() - (i + 1)]
		
		var newCardPosition = global_position + (offset * (cards.size() - (i + 1)))
		card.z_index = i
		
		var arrangeCurve := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
		
		var moveAction := TranslateAction.new(false, false, "ArrangeDiscard", 0.1, 1.5, false, cards[i], newCardPosition, arrangeCurve)
		
		actionList.PushBack(moveAction)

func HideCards() -> void:
	for card in cards:
		card.ShowBack()
		
func ShowCards() -> void:
	for card in cards:
		card.ShowFace()

func AddCard(card : CardBase) -> void:
	cards.push_back(card)
	OffsetCards()

func GrabCard(card: CardBase) -> void:
	
	AddCard(card)
	
	var grabCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
	var moveAction := TranslateAction.new(false, false, "DiscardCard", 1.0, 0.0, false, card, global_position, grabCurve)
	
	actionList.PushBack(moveAction)
	
	var rotateAction := RotateAction.new(true, false, "DiscardCard", 1.0, 0.0, false, card, 0.0, grabCurve)
	
	actionList.PushBack(rotateAction)
	
	OffsetCards()
	
func GetTopCard() -> CardBase:
	return cards.back()
	
func RemoveTopCard() -> CardBase:
	var card = GetTopCard()
	cards.pop_back()
	return card
	
func Shuffle() -> void:
	cards.shuffle()
	OffsetCards()
