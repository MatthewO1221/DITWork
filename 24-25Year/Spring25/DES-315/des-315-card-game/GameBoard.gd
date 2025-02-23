class_name GameBoard
extends Node2D

var delay := 0.1


@export var cardCount := 7

@onready var deck = $DeckContainer
@onready var hand = $PlayerHand

var actionList := ActionList.new()

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	deck.SpawnDeck()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta)


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Deal"):
		DealHands()
	if event.is_action_pressed("Shuffle"):
		ShuffleDeck()
	if event.is_action_pressed("FlipHand"):
		FlipHand()

func DealHands() -> void:
	
	var fakeHands = get_tree().get_nodes_in_group("HandContainer")
	
	var hands : Array[HandContainer]
	
	for hand in fakeHands:
		hands.push_back(hand as HandContainer)
		
	var dealCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
	var dealAction = DealHandsAction.new(false, false, 1, 1.0, 0.0, false, hands, cardCount, deck, 0.1, dealCurve)
	
	actionList.PushBack(dealAction)

func ShuffleDeck() -> void:
	
	var shuffleCurve := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN)
	
	var shuffleAction = ShuffleAction.new(true, false, 1, 1.0, 0.0, false, deck, shuffleCurve)
	
	actionList.PushBack(shuffleAction)
	
func FlipHand() -> void:
	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN)
	
	for card in hand.hand:
		var flipAction = FlipCardAction.new(true, false, 1, 1.0, 0.0, false, card, easingMethod)
		actionList.PushBack(flipAction)
