class_name FlipAction
extends InstantAction





func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupName: String, 
delayedFor: float,
repeats: bool,
affects: CardBase,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupName, delayedFor, repeats, affects, parentAction)
	

func Start() -> void:
	# Simply flips the card when called
	entity.Flip()

func GetActionType() -> String:
	return "Flip"
