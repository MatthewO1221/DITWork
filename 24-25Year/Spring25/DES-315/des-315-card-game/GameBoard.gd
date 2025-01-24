class_name GameBoard
extends Node2D

var delay := 0.1
var cardCount = 0
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	delay -= delta
	
	if delay <= 0.0 and cardCount < 52:
		delay = 0.1
		cardCount += 1
		var newCard = CardFactory.CreateCard(CardBase.Values.Two, CardBase.Suits.Heart)
		add_child(newCard)
		newCard.global_position = get_node("DeckContainer").global_position
