class_name HurtboxComponent
extends Node



@export var damage := 0

var collider: Area2D


signal collided(hurtbox: HurtboxComponent, entity: Node)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	collider = $Area2D
	
	collider.connect("body_entered", OnCollision)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func OnCollision(otherArea: Area2D) -> void:
	var otherHurtbox = otherArea.get_parent() as HurtboxComponent
	
	if !otherHurtbox:
		assert("Could not find hurtbox attached to Area2D")
	
	var otherEntity = otherHurtbox.get_parent() as Node
	
	
	emit_signal("collided", otherHurtbox, otherEntity)
	
		
	var healthComponent = get_parent().get_meta("Health") as HealthComponent
	
	if !healthComponent:
		assert("Could not find health component")
		
	healthComponent.TakeDamage(otherHurtbox.damage, otherEntity)
