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

func DealHands() -> void:
	
	var fakeHands = get_tree().get_nodes_in_group("HandContainer")
	
	var hands : Array[HandContainer]
	
	for hand in fakeHands:
		hands.push_back(hand as HandContainer)
	
	var dealAction = DealHandsAction.new(false, false, 1, 1.0, 0.0, false, hands, cardCount, deck, 0.1)
	
	actionList.PushBack(dealAction)

func ShuffleDeck() -> void:
	var shuffleAction = ShuffleAction.new(true, false, 1, 1.0, 0.0, false, deck)
	
	actionList.PushBack(shuffleAction)
