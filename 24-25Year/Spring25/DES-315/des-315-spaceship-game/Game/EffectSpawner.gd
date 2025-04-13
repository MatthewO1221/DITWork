extends Node



var explosionEffect : PackedScene = preload("res://Ships/Components/ExplosionEffect.tscn")

func SpawnExplosion(position : Vector2, scale : Vector2, strength : float) -> void:
	var explosion = explosionEffect.instantiate()
	
	get_tree().current_scene.add_child(explosion)
	
	explosion.global_position = position
	
	explosion.scale *= scale
	
	explosion.randomStrength = strength
	
	explosion.Explode()
