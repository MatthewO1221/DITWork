class_name HealthPickup
extends PowerupBase


func OnPickup(otherArea: Area2D) -> void:
	super(otherArea)
	
	playerHealth.maxHealth += boostAmount
	
	playerHealth.Heal(boostAmount, self)
	
	queue_free()
