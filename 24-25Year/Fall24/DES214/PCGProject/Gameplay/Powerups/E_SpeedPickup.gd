class_name SpeedPowerup
extends PowerupBase


func _ready() -> void:
	super()
	
	TelemetrySystem.connect_signal(pickupGrabbed, func(_a, lambda: Callable) -> void: lambda.call(), "SpeedPowerupsGrabbed")

func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerBody.speed += boostAmount
	
	queue_free()
