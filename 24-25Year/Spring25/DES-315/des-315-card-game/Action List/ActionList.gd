class_name ActionList
extends Object


var lists : Dictionary

var maxGroup := 0

func UpdateAllActions(delta: float) -> void:
	if lists.is_empty():
		return
	for key in lists.keys():
		var actions = lists[key] as Array[Action]
		var temp : Array[Action] = []
		
		temp.append_array(actions)
			
		for action in temp:
			if action.Update(delta):
				actions.erase(action)
			if action.blocking:
				break
			if action.breaking:
				return

func PushBack(action: Action) -> void:
	var group = action.group
	
	if lists.has(group):
		lists[group].push_back(action)
	else:
		CreateNewList(group).push_back(action)
	
func PushFront(action: Action) -> void:
	var group = action.group
	
	if lists.has(group):
		lists[group].push_front(action)
	else:
		CreateNewList(group).push_front(action)

func IsEmpty() -> bool:
	return lists.is_empty()

func CreateNewList(group: String) -> Array[Action]:
	
	var newArray : Array[Action] = []
	lists[group] = newArray
	
	return lists[group] 
