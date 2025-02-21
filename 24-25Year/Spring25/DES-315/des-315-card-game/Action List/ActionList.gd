class_name ActionList
extends Object


var lists : Dictionary

var maxGroup := 0

func UpdateAllActions(delta: float) -> void:
	for i in range(maxGroup + 1):
		if lists.has(i):
			var actions = lists[i] as Array[Action]
			var temp = actions
			
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

func CreateNewList(group: int) -> Array[Action]:
	if group > maxGroup:
		maxGroup = group
	
	var newArray : Array[Action] = []
	lists[group] = newArray
	
	return lists[group] 
