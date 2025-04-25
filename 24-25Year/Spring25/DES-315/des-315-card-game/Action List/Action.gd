class_name Action
extends Node

##Whether this action blocks others in its group
var blocking : bool
##Whether this action blocks all other actions, regardless of group
var breaking : bool

##The group that this action is in
var group : String

##The delay before this action should begin, in seconds
var delay: float

##Whether this action has a delay
var delayed : bool = true
var initialDelay: float

##Whether this action repeats
var repeating: bool

##This action's parent
var parent : Action
##This action's children
var children : Array[Action]

##Whether the action has started
var started := false

##Highest level action init function
func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
delayedFor: float,
repeats: bool,
parentAction: Action = null) -> void:
	blocking = blocksGroups
	breaking = blocksEverything
	group = groupName
	delay = delayedFor
	repeats = repeating
	parent = parentAction
	
	initialDelay = delayedFor
	
	#Create a timer to delay the action before it starts
	Engine.get_main_loop().create_timer(delayedFor).timeout.connect(FinishedDelay)


##Simply sets delayed to false
func FinishedDelay() -> void:
	delayed = false

##Handles all tick by tick logic for the action, returns true if the action is finished and can be destroyed
func Update(delta: float) -> bool:
	
	#If the action is delayed stop
	if delayed:
		return false
	
	#If this is the first tick of the action
	if not started:
		started = true
		Start()
	
	#Have to do temp array shenanigans because of removals while iterating
	var temp : Array[Action] = []
	
	temp.append_array(children)
	
	#Call update on all children
	for child in temp:
		if child.Update(delta):
			children.erase(child)
		if child.blocking:
			break
	
	#If this action is done
	if ActionFinished():
		End()
		return true
		
	return false


##Timer reset
func ResetTimer() -> void:
	delay = initialDelay

##Contains logic for whether action is done, is overwritten by child classes
func ActionFinished() -> bool:
	if children.is_empty():
		return true
	return false

## Returns array of children
func GetChildren() -> Array[Action]:
	return children

## Get all of this actions children
func GetSiblings() -> Array[Action]:
	assert(parent != null, "Action does not have a parent")
	
	var allChildren = parent.GetChildren()
	var siblings : Array[Action]
	
	siblings.append_array(allChildren)
	
	for child in allChildren:
		if child == self:
			siblings.erase(child)
			
			
	
	return siblings

##Contains logic that an action should perform during its first tick
func Start() -> void:
	pass

func End() -> void:
	pass

func GetActionType() -> String:
	return "Action"
