class_name HurtboxComponent
extends Node2D



@export var damage := 0

var collider: Area2D


signal collided(hurtbox: HurtboxComponent, entity: Node)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	collider = $Area2D
	
	collider.connect("area_entered", OnCollision)
	
	
	

func OnCollision(otherArea: Area2D) -> void:
	var otherHurtbox = otherArea.get_parent() as HurtboxComponent
	
	
	
	var otherEntity = otherHurtbox.get_parent() as Node
	
	
	collided.emit(otherHurtbox, otherEntity)
	
		
