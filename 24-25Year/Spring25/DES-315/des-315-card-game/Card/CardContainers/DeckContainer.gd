class_name DeckContainer
extends CardContainer

@onready var area = $Area2D

@export var offset: Vector2


var cards: Array[CardBase]



# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func SpawnDeck() -> void:
	cards.append_array(CardFactory.GenerateDeck())
	for card in cards:
		card.global_position = global_position
		card.ShowBack()
	
	OffsetCards()
	Shuffle()
	
	


	
func OffsetCards() -> void:
	
	for i in cards.size():
		var card = cards[cards.size() - (i + 1)]
		
		card.global_position = global_position + (offset * (cards.size() - (i + 1)))
		card.z_index = i

func HideCards() -> void:
	for card in cards:
		card.ShowBack()

func AddCard(card : CardBase) -> void:
	cards.push_back(card)
	OffsetCards()
	HideCards()

func DealCard(container : HandContainer) -> void:
	var card = cards.pop_back()
	
	container.AddCard(card)
	
	var dealCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
	container.actionList.PushBack(ArrangeHandAction.new(false, false, "DealCard", 1.0, 0.0, false, container, dealCurve))
	
	
func GetTopCard() -> CardBase:
	return cards.back()
	
func RemoveTopCard() -> CardBase:
	var card = GetTopCard()
	cards.pop_back()
	return card
	
func Shuffle() -> void:
	cards.shuffle()
	OffsetCards()
