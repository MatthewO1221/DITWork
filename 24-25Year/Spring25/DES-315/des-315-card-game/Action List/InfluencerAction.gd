class_name InfluencerAction
extends Action



var entity: Node2D

var actionFunction: Callable


func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
lastsFor: float,
delayedFor: float,
repeats: bool,
affects: Node2D,
action: Callable,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, lastsFor, delayedFor, repeats, parentAction)
	
	entity = affects
	
	actionFunction = action


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
	
	if ActionFinished():
		End()
		return true
		
	return false



func ActionFinished() -> bool:
	
	if !entity.is_inside_tree() and children.is_empty():
		return true

	return super()
