class_name SpeedPowerup
extends PowerupBase


func _ready() -> void:
	super()
	
	TelemetrySystem.connect_signal(self, "pickupGrabbed", "SpeedPowerupsGrabbed")

func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerBody.speed += boostAmount
	
	queue_free()
