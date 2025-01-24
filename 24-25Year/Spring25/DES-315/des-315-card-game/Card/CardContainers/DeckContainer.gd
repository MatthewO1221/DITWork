class_name DeckContainer
extends Node2D

@onready var area = $Area2D

@export var offset: Vector2


var cards: Array[CardBase]

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass



func _on_area_2d_area_entered(area: Area2D) -> void:
	var card = area.get_parent() as CardBase
	
	if !card.is_in_group("Card"):
		return
		
	cards.push_front(card)
	
	OffsetCards()
	
func OffsetCards() -> void:
	
	for i in cards.size():
		var card = cards[cards.size() - (i + 1)]
		
		card.global_position = global_position + (offset * (cards.size() - (i + 1)))
		card.z_index = i
