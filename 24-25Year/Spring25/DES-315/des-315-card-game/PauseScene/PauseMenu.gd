class_name PauseMenu
extends Control

## Pause menu's action list
var actionList := ActionList.new()

## Reference to board controller
var boardController : GameBoard

## Sent when pause menu is active
signal InPlace

## Sent when pause menu is inactive
signal OutofPlace

func _ready() -> void:
	boardController = get_parent() as GameBoard

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Pause"):
		Pause()


func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta)

## Moves the pause menu into place
func Pause() -> void:
	# Pause everything
	get_tree().paused = true
	
	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_BOUNCE, Tween.EaseType.EASE_OUT)
	var translateAction := UITranslateAction.new(false, false, "PauseIn", 1.0, 0.0, false, self, Vector2.ZERO, easingMethod)
	
	actionList.PushBack(translateAction)

## Moves the pause menu off screen
func Unpause() -> void:
	get_tree().paused = false
	
	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_BOUNCE, Tween.EaseType.EASE_IN)
	var translateAction := UITranslateAction.new(false, false, "PauseIn", 1.0, 0.0, false, self, Vector2(1500, 0), easingMethod)

	actionList.PushBack(translateAction)
	
## Called when quit button is pressed
func Quit() -> void:
	get_tree().quit()

## Called when play speed is updated
func UpdatePlaySpeed(changed: bool) -> void:
	if changed:
		boardController.UpdatePlaySpeed($HFlowContainer/PlaySpeed.value)
		
## Called when hand size is updated
func UpdateHandSize(changed: bool) -> void:
	if changed:
		boardController.UpdateHandSize($HFlowContainer/HandSize.value)

## Called when hand number is updated
func UpdateHandNumber(changed: bool) -> void:
	if changed:
		boardController.UpdateHandNumber($HFlowContainer/HandNumber.value)
