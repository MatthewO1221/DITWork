class_name Bullet
extends Ammunition

@export_category("Bullet Variables")
@export var lifeTime : float = 10.0


func _process(delta: float) -> void:
	lifeTime -= delta
	
	if lifeTime <= 0.0:
		queue_free()



func Hit(body: Node2D) -> void:
	queue_free()
