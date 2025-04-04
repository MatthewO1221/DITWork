class_name InfluencerAction
extends Action



var entity: Node2D

var actionFunction: Callable

var curve : CustomCurve

var duration : float
var timePassed := 0.0

var timer : SceneTreeTimer

func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
action: Callable,
easingMethod : CustomCurve,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, delayedFor, repeats, parentAction)
	
	entity = affects
	
	actionFunction = action
	
	curve = easingMethod
	
	duration = lastsFor



func Update(delta: float) -> bool:
	
	
	if delayed:
		return false
	
	if not started:
		started = true
		Start()
	
	UpdateTimePassed(delta)
	
	actionFunction.call()
	
	var temp : Array[Action] = []
	
	temp.append_array(children)
	
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
	


func UpdateTimePassed(delta: float) -> void:
	var timeScale = Engine.time_scale
	
	timePassed += delta
