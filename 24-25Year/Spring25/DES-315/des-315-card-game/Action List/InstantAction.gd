class_name InstantAction
extends Action



var entity: Node2D


func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
delayedFor: float,
repeats: bool,
affects: Node2D,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, delayedFor, repeats, parentAction)
	
	entity = affects
	
	


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



func ActionFinished() -> bool:
	
	if children.is_empty():
		if !entity.is_inside_tree():
			return true
		if started:
			return true

	return false
