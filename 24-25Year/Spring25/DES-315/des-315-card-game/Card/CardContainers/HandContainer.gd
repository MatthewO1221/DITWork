class_name HandContainer
extends Node2D


var hand : Array[CardBase]

var actionList := ActionList.new()

@export var spacing := 100.0
@export var angle := 30.0
@export var height := 50.0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta)


func HandArrangement(numCards : int) -> Array[Transform2D]:
	var layout : Array[Transform2D] = []
	
	if numCards == 0:
		return layout
	
	var width : float = (numCards - 1) * spacing
	var center : float  = width / 2
	
	for i in range(numCards):
		var xPos = i * spacing - center
		
		var normalized: float = 0.0
		if width > 0.0:
			normalized = abs(xPos) / width
		else:
			normalized = 0.0
		
		var yOffset : float = -height * (1.0 - pow(normalized, 2))
		var yPos : float = global_position.y + yOffset
		
		var curAngle = -angle/2
		
		if numCards > 1:
			curAngle += angle * i / (numCards - 1)
		
		var newTransform := Transform2D(deg_to_rad(curAngle), Vector2(xPos, yPos))
		
		layout.push_back(newTransform)
	
	return layout

	
func GetDealPosition() -> Transform2D:
	var newLayout = HandArrangement(hand.size() + 1)
	
	return newLayout.back()


func AddCard(card : CardBase) -> void:
	hand.push_back(card)

func RemoveCard(card : CardBase) -> void:
	hand.erase(card)
