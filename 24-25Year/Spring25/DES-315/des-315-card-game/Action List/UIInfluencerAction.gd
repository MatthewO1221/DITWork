class_name UIInfluencerAction
extends Action



var entity: Control

var actionFunction: Callable

var curve : CustomCurve

var duration : float
var timePassed := 0.0

func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Control,
action: Callable,
easingMethod : CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, delayedFor, repeats, parentAction)
	
	entity = affects
	
	actionFunction = action
	
	curve = easingMethod
	
	duration = lastsFor

func IncrementTimer(delta: float) -> void:
	if delay > 0.0:
		delay -= delta
	else:
		timePassed += delta


func Update(delta: float) -> bool:
	
	
	IncrementTimer(delta)
	
	if delay > 0.0:
		return false
	
	if not started:
		started = true
		Start()
	
	actionFunction.call()
	
	var temp = children
	
	for child in temp:
		if child.Update(delta):
			children.erase(child)
		if child.blocking:
			break
	
	if ActionFinished():
		End()
		return true
		
	return false



func ActionFinished() -> bool:
	
	if children.is_empty():
		if !entity.is_inside_tree():
			return true
		if timePassed >= duration and !repeating:
			return true

	return false

func TimeLeft() -> float:
	return duration - timePassed
	
func ResetTimer() -> void:
	timePassed = 0.0
	delay = initialDelay

func GetPercentDone() -> float:
	return timePassed / duration
