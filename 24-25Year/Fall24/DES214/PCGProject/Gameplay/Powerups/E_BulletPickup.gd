class_name BulletPowerup
extends PowerupBase


func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerWeapon.numBullets += boostAmount
	
	queue_free()
