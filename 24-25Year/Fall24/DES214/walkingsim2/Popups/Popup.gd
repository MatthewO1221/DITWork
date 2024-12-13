class_name PopupTemp
extends Control

var text: String
var time: float

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	$ColorRect/Label.text = text
	$Timer.start(time)



func Init(textToDisplay: String, lifeTime: float) -> void:
	text = textToDisplay
	time = lifeTime
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _on_Timer_timeout():
	queue_free()
