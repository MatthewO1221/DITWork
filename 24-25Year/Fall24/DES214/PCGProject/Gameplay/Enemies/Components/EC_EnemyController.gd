class_name EnemyController
extends CharacterBody2D


@export_category("Enemy Variables")
@export var moveSpeed := 50.0
@export var checkDistance := 1000.0
@export var chaseDistance := 1500.0


var playerDetected: bool
var player: Node
var weapon: EnemyWeaponBase
var timer: Timer
var raycast: RayCast2D
var health: HealthComponent

var reloaded := false


func _ready() -> void:
	GetPlayer.call_deferred()
	timer = get_node(get_meta("Timer")) as Timer
	weapon = get_node(get_meta("Weapon")) as EnemyWeaponBase
	raycast = get_node(get_meta("Raycast")) as RayCast2D
	health = get_node(get_meta("Health")) as HealthComponent
	
	timer.timeout.connect(CheckPlayerDistance)
	weapon.Reloaded.connect(Reloaded)
	health.deathSignal.connect(OnDeath)
	
	raycast.enabled = false
	
func GetPlayer() -> void:
	player = get_tree().get_nodes_in_group("Player")[0] if get_tree().has_group("Player") else null

func _process(delta: float) -> void:
	
	if reloaded and playerDetected:
		weapon.Fire()
		reloaded = false
	
func CheckPlayerDistance() -> void:
	
	var distance = Vector2(player.global_position - global_position).length()
	
	if playerDetected == false and distance <= checkDistance:
		raycast.enabled = true
		raycast.global_position = global_position
		raycast.target_position = player.global_position
		
		if !raycast.is_colliding():
			playerDetected = true
	elif playerDetected == true and distance >= chaseDistance:
		playerDetected = false
		
	raycast.enabled = false

func Reloaded() -> void:
	reloaded = true
	

func _physics_process(delta: float) -> void:
	
	if playerDetected:
		velocity = GetMovement()
	
		move_and_slide()
	
func GetMovement() -> Vector2:
	var toPlayer = Vector2(player.global_position - global_position).normalized()
	
	toPlayer = toPlayer * moveSpeed
	
	return toPlayer
	
func OnDeath(_killer: Node) -> void:
	queue_free()
	
