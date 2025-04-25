class_name CardBase
extends Node2D

# Enum which contains all suits
enum Suits
{
	Diamond,
	Heart,
	Spade,
	Club
}

# Map of suits enum to strings
const suitMap := {
	Suits.Diamond : "Diamond",
	Suits.Heart : "Heart",
	Suits.Spade : "Spade",
	Suits.Club : "Club"
	}

# Enum which contains all values
enum Values
{
	Two = 2,
	Three = 3,
	Four = 4,
	Five = 5,
	Six = 6,
	Seven = 7,
	Eight = 8,
	Nine = 9,
	Ten = 10,
	Jack = 11,
	Queen = 12,
	King = 13,
	Ace = 14
	
}

# Map of values to strings
const valueMap := {
	Values.Two : "2",
	Values.Three : "3",
	Values.Four : "4",
	Values.Five : "5",
	Values.Six : "6",
	Values.Seven : "7",
	Values.Eight : "8",
	Values.Nine : "9",
	Values.Ten : "10",
	Values.Jack : "J",
	Values.Queen : "Q",
	Values.King : "K",
	Values.Ace : "A"
}


## The value of this card
var value : Values
## The suit of this card
var suit : Suits

## The face texture of this card
var cardTexture : Resource

## The back texture, same for all cards
var backTexture = preload("res://Card/CardSprites/card_back.png")

## Whether this card is face up
var showingFace := true

## Sent when the mouse goes over the card
signal mouseEntered(CardBase)
## Sent when the mouse is no longer over the card
signal mouseExited(CardBase)
## Sent when the card is clicked
signal clicked(CardBase)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	# Get the texture from the card factory
	cardTexture = CardFactory.GetTexture(value, suit)

## Returns the name of the card formatted as value of suit
func GetCardName() -> String:
	return valueMap[value] + " of " + suitMap[suit]

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

## Changes to either the face or back texture
func Flip() -> void:
	if !showingFace:
		$Sprite2D.texture = cardTexture
		showingFace = true
	else:
		$Sprite2D.texture = backTexture
		showingFace = false

## Makes the card face down
func ShowBack() -> void:
	$Sprite2D.texture = backTexture
	showingFace = false

## Makes the card face up
func ShowFace() -> void:
	$Sprite2D.texture = cardTexture
	showingFace = true


func SendMouseEntered() -> void:
	mouseEntered.emit(self)
	
func SendMouseExited() -> void:
	mouseExited.emit(self)

## Emits clicked signal
func _on_area_2d_input_event(viewport: Node, event: InputEvent, shape_idx: int) -> void:
	if event.is_action_pressed("Clicked"):
	
		clicked.emit(self)
