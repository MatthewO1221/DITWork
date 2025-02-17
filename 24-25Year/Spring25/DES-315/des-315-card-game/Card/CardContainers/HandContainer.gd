class_name HandContainer
extends Node2D


var hand : Array[CardBase]

var actionList := ActionList.new()

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func ArrangeHand() -> void:
	pass
	
func GetNextPosition() -> Transform2D:
	pass
