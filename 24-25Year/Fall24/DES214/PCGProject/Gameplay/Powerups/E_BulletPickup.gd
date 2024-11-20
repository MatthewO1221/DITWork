class_name BulletPowerup
extends PowerupBase


func _ready() -> void:
	super()
	
	TelemetrySystem.connect_signal(pickupGrabbed, func(_a, lambda: Callable) -> void: lambda.call(), "BulletPowerupsGrabbed")

func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerWeapon.numBullets += boostAmount
	
	queue_free()
