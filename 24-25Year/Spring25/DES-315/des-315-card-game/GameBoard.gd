class_name GameBoard
extends Node2D

var delay := 0.1
var cardCount = 0



# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var deck = $DeckContainer
	deck.SpawnDeck()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
