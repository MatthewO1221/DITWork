extends Node

const cardScene = preload("res://Card/Card.tscn")

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func CreateCard(value: CardBase.Values, suit: CardBase.Suits) -> Node2D:
	var newCard = cardScene.instantiate()
	assert(newCard != null, "Failed to create card of type: " + CardBase.valueMap[value] + " of " + CardBase.suitMap[suit])
	newCard.value = value
	newCard.suit = suit
	
	var cardSprite = newCard.get_node("Sprite2D") as Sprite2D
	
	var textureString = "res://Card/CardSprites/" + CardBase.suitMap[suit] + "_" + CardBase.valueMap[value] + ".png"
	
	var cardTexture = load(textureString)
	
	cardSprite.texture = cardTexture
	
	return newCard

func CreateRandomCard() -> Node2D:
	var randValue = CardBase.Suits.values().pick_random()
	var randSuit = CardBase.Values.values().pick_random()
	
	return CreateCard(randValue, randSuit)
