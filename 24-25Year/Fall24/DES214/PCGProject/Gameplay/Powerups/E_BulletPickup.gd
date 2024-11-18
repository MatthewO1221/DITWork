class_name BulletPowerup
extends PowerupBase


func _ready() -> void:
	super()
	
	TelemetrySystem.connect_signal(self, "pickupGrabbed", "BulletPowerupsGrabbed")

func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerWeapon.numBullets += boostAmount
	
	queue_free()
