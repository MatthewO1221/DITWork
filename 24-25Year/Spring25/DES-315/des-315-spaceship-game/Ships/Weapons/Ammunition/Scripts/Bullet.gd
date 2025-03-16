class_name Bullet
extends Ammo


@export var lifeTime : float = 10.0
@export var damage : int = 5


func _process(delta: float) -> void:
	lifeTime -= delta
	
	if lifeTime <= 0.0:
		queue_free()








func _on_body_entered(body: Node) -> void:
	var health = body.get_node(body.get_meta("Health")) as Health
	
	if health != null:
		health.Damage(damage)
		
		
		
	queue_free()
