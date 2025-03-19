class_name Action
extends Node


var blocking : bool
var breaking : bool

var group : String


var delay: float
var delayed : bool = true
var initialDelay: float
var repeating: bool


var parent : Action
var children : Array[Action]


var started := false

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
	
	
	Engine.get_main_loop().create_timer(delayedFor).timeout.connect(FinishedDelay)


func FinishedDelay() -> void:
	delayed = false

func Update(delta: float) -> bool:
	
	
	if delayed:
		return false
	
	if not started:
		started = true
		Start()
	
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



func ResetTimer() -> void:
	delay = initialDelay

func ActionFinished() -> bool:
	if children.is_empty():
		return true
	return false


func GetChildren() -> Array[Action]:
	return children

func GetSiblings() -> Array[Action]:
	assert(parent != null, "Action does not have a parent")
	
	var allChildren = parent.GetChildren()
	var siblings : Array[Action]
	
	siblings.append_array(allChildren)
	
	for child in allChildren:
		if child == self:
			siblings.erase(child)
			
			
	
	return siblings

func Start() -> void:
	pass

func End() -> void:
	pass

func GetActionType() -> String:
	return "Action"
