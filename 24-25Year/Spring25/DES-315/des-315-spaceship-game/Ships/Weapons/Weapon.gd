class_name Weapon
extends Node2D


@export var ammo : PackedScene

func Fire() -> void:
	pass
	
	
func SpawnAmmo() -> Node:
	var projectile = ammo.instantiate()
	
	add_child(projectile)
	
	return projectile
