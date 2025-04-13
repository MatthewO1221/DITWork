class_name BossBehavior
extends CharacterBody2D

enum States
{
	Searching,
	Seeking,
	Orbiting
}

@export var orbitDistance : float = 1000.0
@export var seekSpeed : float = 100.0
@export var orbitSpeed : float = 100.0

var curState : States = States.Searching
var player : PlayerShip = null

func _ready() -> void:
	player = get_tree().get_first_node_in_group("PlayerShip")
	
	if player != null:
		curState = States.Seeking


func _physics_process(delta: float) -> void:
	
	match curState:
		States.Searching:
			Search()
		States.Seeking:
			Seek()
		States.Orbiting:
			Orbit()
			
			
func Search() -> void:
	player = get_tree().get_first_node_in_group("PlayerShip")
	
	if player != null:
		curState = States.Seeking
		
func Seek() -> void:
	if !is_instance_valid(player):
		curState = States.Searching
		return
	
	var distToPlayer = global_position.distance_to(player.global_position)
	
	if distToPlayer > orbitDistance:
		velocity = (player.global_position - global_position).normalized() * seekSpeed
	elif distToPlayer <= orbitDistance:
		velocity = Vector2.ZERO
		curState = States.Orbiting
		
	var direction = (player.global_position - global_position).normalized()
	rotation = direction.angle() + deg_to_rad(90)  # +90° because -Y is forward
	
	move_and_slide()
	
	
func Orbit() -> void:
	if !is_instance_valid(player):
		curState = States.Searching
		return
	
	var toPlayer := global_position.direction_to(player.global_position)
	
	var newVelocity : Vector2 = Vector2.ZERO
	
	newVelocity += Vector2(toPlayer.y, -toPlayer.x) * orbitSpeed
	
	if global_position.distance_to(player.global_position) < orbitDistance:
		newVelocity += -toPlayer * orbitSpeed
	
	velocity = newVelocity
	
	var direction = (player.global_position - global_position).normalized()
	rotation = direction.angle() + deg_to_rad(90)  # +90° because -Y is forward
	
	move_and_slide()

func Die() -> void:
	EffectSpawner.SpawnExplosion(global_position, Vector2(5,5), 30.0)
	queue_free()
