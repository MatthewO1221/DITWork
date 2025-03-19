class_name PauseMenu
extends Control


var actionList := ActionList.new()

var boardController : GameBoard

signal InPlace
signal OutofPlace

func _ready() -> void:
	boardController = get_parent() as GameBoard

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Pause"):
		Pause()


func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta)


func Pause() -> void:
	get_tree().paused = true
	
	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_BOUNCE, Tween.EaseType.EASE_OUT)
	var translateAction := UITranslateAction.new(false, false, "PauseIn", 1.0, 0.0, false, self, Vector2.ZERO, easingMethod)
	
	actionList.PushBack(translateAction)
	
func Unpause() -> void:
	get_tree().paused = false
	
	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_BOUNCE, Tween.EaseType.EASE_IN)
	var translateAction := UITranslateAction.new(false, false, "PauseIn", 1.0, 0.0, false, self, Vector2(1000, 0), easingMethod)

	actionList.PushBack(translateAction)
	
	
func Quit() -> void:
	get_tree().root.propagate_notification(NOTIFICATION_WM_CLOSE_REQUEST)


func UpdatePlaySpeed(changed: bool) -> void:
	if changed:
		boardController.playSpeed = $HFlowContainer/PlaySpeed.value

func UpdateHandSize(changed: bool) -> void:
	if changed:
		boardController.handSize  = $HFlowContainer/HandSize.value

func UpdateHandNumber(changed: bool) -> void:
	if changed:
		boardController.botNum = $HFlowContainer/HandNumber.value
