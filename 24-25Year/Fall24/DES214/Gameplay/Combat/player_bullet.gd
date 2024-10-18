extends RigidBody2D

var speed: float = 500.0  # Bullet speed 
var damage: float = 10.0 # Damage inflicted by the bullet
var size: Vector2i = Vector2i(1,1)
var direction: float

func _ready() -> void:
	# Connect the body_entered signal to detect collisions
	connect("body_entered", _on_bullet_body_entered)
	# Give the bullet an initial velocity
	linear_velocity = Vector2(speed, 0).rotated(rotation)

func _on_bullet_body_entered(body: Node) -> void:
	# Check if the body is on the 'Enemy' collision layer and has a health component
	if body.is_in_group("Enemy") and body.has_method("apply_damage"):
		#body.get_child(bulletDamage)
		queue_free()  # Destroy the bullet after hitting the enemy
	else:
		queue_free()  # If it hits anything else, destroy the bullet

func _process(delta: float) -> void:
	# Free the bullet if it goes too far off screen
	if not get_viewport_rect().has_point(global_position):
		queue_free()
