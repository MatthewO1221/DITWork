class_name ActionList
extends Object

var actions: Array[Action]

func UpdateAllActions(delta: float) -> void:
	for action in actions:
		if action.Update(delta):
			actions.erase(action)

func PushBack(action: Action) -> void:
	actions.push_back(action)

func IsEmpty() -> bool:
	return actions.is_empty()
