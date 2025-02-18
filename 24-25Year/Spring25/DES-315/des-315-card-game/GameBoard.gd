class_name GameBoard
extends Node2D

var delay := 0.1
var cardCount = 0

@onready var deck = $DeckContainer
@onready var hand = $HandContainer

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	deck.SpawnDeck()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Deal"):
		deck.DealCard(hand)
