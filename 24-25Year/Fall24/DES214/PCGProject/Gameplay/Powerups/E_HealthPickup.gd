class_name HealthPickup
extends PowerupBase


func _ready() -> void:
	super()
	
	TelemetrySystem.connect_signal(pickupGrabbed, func(_a, lambda: Callable) -> void: lambda.call(), "HealthPowerupsGrabbed")

func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerHealth.maxHealth += boostAmount
	
	playerHealth.Heal(boostAmount, self)
	
	queue_free()
