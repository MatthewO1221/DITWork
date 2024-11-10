class_name SpeedPowerup
extends PowerupBase


func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerBody.speed += boostAmount
	
	queue_free()
