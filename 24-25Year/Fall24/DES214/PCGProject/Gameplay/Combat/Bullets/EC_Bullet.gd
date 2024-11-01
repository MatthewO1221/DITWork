class_name BulletComponent
extends Node

var bulletDamage := 1
var bulletSize := 1.0
var bulletSpeed := 1.0
var bulletRange := 10.0


var hurtbox: HurtboxComponent

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	get_parent().scale = Vector2(bulletSize,bulletSize)
	
	hurtbox = get_parent().get_meta("Hurtbox")
	
	if !hurtbox:
		assert("Could not find hurtbox component on bullet")

	hurtbox.damage = bulletDamage

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
