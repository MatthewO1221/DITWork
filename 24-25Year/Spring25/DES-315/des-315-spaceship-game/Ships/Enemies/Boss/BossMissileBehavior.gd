class_name BossMissileBehavior
extends BossBehavior

@export var missileLaunchTime : float = 10.0

@onready var missileLaunchTimer : Timer = $Timer



func _ready() -> void:
	super()
	
	missileLaunchTimer.start(missileLaunchTime)
	missileLaunchTimer.timeout.connect(Launch)
	
func _physics_process(delta: float) -> void:
	super(delta)
	
func Launch() -> void:
	$MissileLauncher.Launch()
	missileLaunchTimer.start(missileLaunchTime)
	
