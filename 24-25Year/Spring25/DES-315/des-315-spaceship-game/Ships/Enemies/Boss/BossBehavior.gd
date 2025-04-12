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
	var distToPlayer = global_position.distance_to(player.global_position)
	
	if distToPlayer > orbitDistance:
		velocity = (player.global_position - global_position).normalized() * seekSpeed
	elif distToPlayer <= orbitDistance:
		velocity = Vector2.ZERO
		curState = States.Orbiting
		
	look_at(player.global_position)
	
	
func Orbit() -> void:
	pass
