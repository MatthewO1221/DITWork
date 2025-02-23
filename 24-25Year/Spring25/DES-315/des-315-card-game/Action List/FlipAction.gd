class_name FlipAction
extends InstantAction





func _init(blocksGroups: bool, 
blocksEverything: bool, 
groupNum: int, 
delayedFor: float,
repeats: bool,
affects: CardBase,
parentAction: Action = null) -> void:
	super(blocksGroups, blocksEverything, groupNum, delayedFor, repeats, affects, parentAction)
	

func Start() -> void:
	entity.Flip()
