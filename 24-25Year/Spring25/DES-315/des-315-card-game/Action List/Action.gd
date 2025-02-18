class_name Action
extends Object


var blocking : bool
var benign := false

var timePassed := 0.0
var duration: float
var entity: Node2D
var delay: float
var initialDelay: float
var repeating: bool

var actionFunction: Callable

var parent : Action
var children : Array[Action]




func _init(newDuration: float, newEntity: Node2D, newDelay: float, newRepeating: bool) -> void:
	duration = newDuration
	entity = newEntity
	delay = newDelay
	initialDelay = newDelay
	repeating = newRepeating
	

func IncrementTimer(delta: float) -> void:
	if delay > 0.0:
		delay -= delta
	else:
		timePassed += delta

func Update(delta: float) -> bool:
	
	IncrementTimer(delta)
	if KillCheck():
		return true

	if delay > 0.0:
		return false

	if !benign and timePassed >= duration:
		ResetTimer()
		return false
	
	if !benign:
		actionFunction.call()
	
	var allChildrenDone = true
	
	for child in children:
		if not child.Update(delta):
			allChildrenDone = false
			
	if children.size() > 0 and allChildrenDone == true:
		return true

	return false

func TimeLeft() -> float:
	return duration - timePassed

func ResetTimer() -> void:
	timePassed = duration
	delay = initialDelay

func KillCheck() -> bool:
	if !benign and !entity.is_inside_tree():
		return true

	if !benign and timePassed >= duration and !repeating:
		return true

	return false

func GetPercentDone() -> float:
	return timePassed / duration

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
