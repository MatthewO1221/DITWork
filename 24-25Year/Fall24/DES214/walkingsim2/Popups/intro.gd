extends Control

var fadeSpeed := 0.25
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	
	anchor_left = 0.0
	anchor_top = 0.0
	anchor_right = 1.0
	anchor_bottom = 1.0

	offset_left = 0
	offset_top = 0
	offset_right = 0
	offset_bottom = 0

	position = Vector2.ZERO  # Ensure the position is reset


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	modulate = Color(modulate, modulate.a - (fadeSpeed * delta))
	
	if modulate.a <= 0.0:
		queue_free()
