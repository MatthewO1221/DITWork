class_name CardBase
extends Node2D

enum Suits
{
	Diamond,
	Heart,
	Spade,
	Club
}

const suitMap := {
	Suits.Diamond : "Diamond",
	Suits.Heart : "Heart",
	Suits.Spade : "Spade",
	Suits.Club : "Club"
	}

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


var value : Values
var suit : Suits

var cardTexture : Resource
var backTexture = preload("res://Card/CardSprites/card_back.png")

var showingFace := true

signal mouseEntered(CardBase)
signal mouseExited(CardBase)
signal clicked(CardBase)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	cardTexture = CardFactory.GetTexture(value, suit)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func Flip() -> void:
	if !showingFace:
		$Sprite2D.texture = cardTexture
		showingFace = true
	else:
		$Sprite2D.texture = backTexture
		showingFace = false

func ShowBack() -> void:
	$Sprite2D.texture = backTexture
	showingFace = false
	
func ShowFace() -> void:
	$Sprite2D.texture = cardTexture
	showingFace = true


func SendMouseEntered() -> void:
	mouseEntered.emit(self)
	
func SendMouseExited() -> void:
	mouseExited.emit(self)


func _on_area_2d_input_event(viewport: Node, event: InputEvent, shape_idx: int) -> void:
	if event.is_action_pressed("Clicked"):
	
		clicked.emit(self)
