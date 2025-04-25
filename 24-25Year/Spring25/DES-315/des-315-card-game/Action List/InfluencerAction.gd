## Influencer Actions are the most common type. They effect entities continuously over time. This covers things 
## like translation, scale, or rotate actions
class_name InfluencerAction
extends Action


## The entity being affected
var entity: Node2D

## The function that is called every frame, lambdas for the win
var actionFunction: Callable

## The "curve" that dictates the value every frame
var curve : CustomCurve

## How long the action takes
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
	
	# Leftover from when engine time was having problems, just ticks the time passed
	UpdateTimePassed(delta)
	
	# Call action function
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


## Set of requirements for if the action is considered finished and can be deleted
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
	
	timePassed += delta
