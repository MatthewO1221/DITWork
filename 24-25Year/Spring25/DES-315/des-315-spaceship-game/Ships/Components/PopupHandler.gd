extends CanvasLayer


@onready var textLabel := $PopupText/RichTextLabel


var fadeTimer : float = 5.0

var curFadeTime : float = 0.0

func SetText(newText: String) -> void:
	textLabel.text = newText
	
	curFadeTime = 0.0

func _process(delta: float) -> void:
	
	curFadeTime += delta
	
	if curFadeTime >= fadeTimer:
		visible = false
	else:
		visible = true
