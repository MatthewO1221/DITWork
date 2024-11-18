class_name HealthPickup
extends PowerupBase


func _ready() -> void:
	super()
	
	TelemetrySystem.connect_signal(self, "pickupGrabbed", "HealthPowerupsGrabbed")

func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerHealth.maxHealth += boostAmount
	
	playerHealth.Heal(boostAmount, self)
	
	queue_free()
