extends Node



var explosionEffect : PackedScene = preload("res://Ships/Components/ExplosionEffect.tscn")
var star : PackedScene = preload("res://Game/Star.tscn")

func SpawnExplosion(position : Vector2, scale : Vector2, strength : float) -> void:
	var explosion = explosionEffect.instantiate()
	
	get_tree().current_scene.add_child(explosion)
	
	explosion.global_position = position
	
	explosion.scale *= scale
	
	explosion.randomStrength = strength
	
	explosion.Explode()


func _ready() -> void:
	SpawnStars()

func SpawnStars() -> void:
	
	for x in range(-100000, 100001, 2000):
		for y in range(-100000, 100001, 2000):
			var newStar = star.instantiate()
			
			newStar.global_position = Vector2(x,y)
			
			
			get_tree().current_scene.add_child(newStar)
	
