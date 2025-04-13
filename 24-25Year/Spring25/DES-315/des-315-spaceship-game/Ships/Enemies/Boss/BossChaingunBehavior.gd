class_name BossChaingunBehavior
extends BossBehavior


@export var chaingunFireTime : float = 15.0
@export var chaingunFireInterval : float = 10.0

@onready var fireTimer : Timer = $FireTimer
@onready var intervalTimer : Timer = $IntervalTimer


func _ready() -> void:
	super()
	
	intervalTimer.start(chaingunFireInterval)
	
	intervalTimer.timeout.connect(StartFiring)
	
func _physics_process(delta: float) -> void:
	super(delta)

func Die() -> void:
	super()
	
func StartFiring() -> void:
	$Chaingun.StartFiring()
	fireTimer.start(chaingunFireTime)
	fireTimer.timeout.connect(StopFiring)
	
func StopFiring() -> void:
	$Chaingun.StopFiring()
	intervalTimer.start(chaingunFireInterval)
