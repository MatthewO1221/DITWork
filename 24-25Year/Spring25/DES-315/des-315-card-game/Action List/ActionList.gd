class_name ActionList
extends Object

## All groups of actions are stored via [Group Name : String, Action List : Array[Action]
var lists : Dictionary


var maxGroup := 0

## Updates every action in the action list
func UpdateAllActions(delta: float) -> void:
	# If there are no actions just skip
	if lists.is_empty():
		return
		
	# Go through every group of actions
	for key in lists.keys():
		var actions = lists[key] as Array[Action]
		var temp : Array[Action] = []
		
		#Temp array shenanigans
		temp.append_array(actions)
		
		#Iterate through every action
		for action in temp:
			if action.Update(delta):
				actions.erase(action)
			if action.blocking:
				break
			if action.breaking:
				return

## Logic for adding a new action to the back of the list
func PushBack(action: Action) -> void:
	var group = action.group
	
	#If the group already exists add it, if not create that group
	if lists.has(group):
		lists[group].push_back(action)
	else:
		CreateNewList(group).push_back(action)

## Logic for adding a new action to the front of the list
func PushFront(action: Action) -> void:
	var group = action.group
	
	if lists.has(group):
		lists[group].push_front(action)
	else:
		CreateNewList(group).push_front(action)

func IsEmpty() -> bool:
	return lists.is_empty()

## Creates a new list in the dictionary with key group
func CreateNewList(group: String) -> Array[Action]:
	
	var newArray : Array[Action] = []
	lists[group] = newArray
	
	return lists[group] 
