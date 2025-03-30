class_name Bullet
extends Ammo


@export var lifeTime : float = 10.0
@export var damage : int = 5
@export var knockbackStrength : float = 10.0


func _process(delta: float) -> void:
	lifeTime -= delta
	
	if lifeTime <= 0.0:
		queue_free()








func _on_body_entered(body: Node) -> void:
	# Try to get the Health component
	if body.has_meta("Health"):
		var health = body.get_node(body.get_meta("Health")) as Health
		if health:
			health.Damage(damage)

	# If the body is not a RigidBody2D, check its parent
	var target_body: RigidBody2D = null
	
	if body is RigidBody2D:
		target_body = body
	else:
		var parent := body.get_parent()
		if parent is RigidBody2D:
			target_body = parent

	if target_body == null:
		queue_free()  # No valid rigid body found, so destroy the object
		return

	# Get the collision normal (assumes _on_body_entered is connected to a collision signal)
	var normal := (global_position - target_body.global_position).normalized()

	# Apply impulse using the correct method for Godot 4.x
	target_body.apply_impulse(normal * knockbackStrength)

	# Destroy self after impact
	queue_free()
