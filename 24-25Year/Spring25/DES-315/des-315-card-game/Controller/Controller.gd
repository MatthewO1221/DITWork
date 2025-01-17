class_name Controller
extends Node2D

var actionList: ActionList

var cards: Array[Node2D]
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	cards.append_array(get_tree().get_nodes_in_group("Card"))
	CreateActions()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta)

	if actionList.IsEmpty():
		CreateActions()


func CreateActions() -> void:
	actionList = ActionList.new()

	
	var screenSize = get_viewport().get_visible_rect().size

	for card in cards:
		var randX = randf_range(-screenSize.x, screenSize.x)
		var randY = randf_range(-screenSize.y, screenSize.y)
		var randRot = randf_range(0, 360)
		actionList.PushBack(TranslateAction.new(5.0, card, 2.0, false, Vector2(randX, randY)))
		actionList.PushBack(RotateAction.new(5.0, card, 2.0, false, randRot))
