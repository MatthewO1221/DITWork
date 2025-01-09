class_name Outro
extends Control

var fadein = false
var fadeSpeed := 0.5
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if fadein:
		modulate = Color(modulate, modulate.a + (fadeSpeed * delta))
	
	if modulate.a >= 1.0:
		get_tree().quit()
func _on_timer_timeout() -> void:
	fadein = true
