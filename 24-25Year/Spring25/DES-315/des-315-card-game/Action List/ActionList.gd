class_name ActionList
extends Object

var actions: Array[Action]

func UpdateAllActions(delta: float) -> void:
    for action in actions:
        action.IncrementTimer(delta)
        if action.Update():
            actions.erase(action)

func PushBack(action: Action) -> void:
    actions.push_back(action)

func IsEmpty() -> bool:
    return actions.is_empty()