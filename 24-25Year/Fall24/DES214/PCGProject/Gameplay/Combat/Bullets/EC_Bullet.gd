class_name BulletComponent
extends CharacterBody2D

var bulletDamage := 1
var bulletSize := 1.0
var bulletSpeed := 1.0
var bulletRange := 10.0
var bounces := 0


var hurtbox: HurtboxComponent

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	
	self.scale = scale * bulletSize
	
	hurtbox = get_node(get_meta("Hurtbox")) as HurtboxComponent
	
	if !hurtbox:
		assert("Could not find hurtbox component on bullet")

	hurtbox.damage = bulletDamage
	
	hurtbox.collided.connect(OnCollision)
	

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	move_and_slide()
	
	bulletRange -= velocity.length()
	
	if bulletRange <= 0.0:
		queue_free()
	
	var collision = get_last_slide_collision()
	
	if collision and bounces > 0:
		velocity.bounce(collision.get_normal())
		bounces -= 1
	elif collision and bounces == 0:
		queue_free()


func OnCollision(hurtbox: HurtboxComponent, source: Node) -> void:
	queue_free()
	
