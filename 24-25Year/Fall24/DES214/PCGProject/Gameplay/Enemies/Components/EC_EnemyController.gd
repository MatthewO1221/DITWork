class_name EnemyController
extends CharacterBody2D


@export_category("Enemy Variables")
@export var moveSpeed := 50.0
@export var checkDistance := 1000.0
@export var enemyName : String


var playerDetected := false
var player: Node
var weapon: EnemyWeaponBase
var health: HealthComponent
var detectionArea: Area2D

var reloaded := false

var deathIcon = "res://Gameplay/Enemies/E_Corpse.tscn"

func _ready() -> void:
	GetPlayer.call_deferred()
	detectionArea = get_node(get_meta("Detector")) as Area2D
	weapon = get_node(get_meta("Weapon")) as EnemyWeaponBase
	health = get_node(get_meta("Health")) as HealthComponent
	
	weapon.Reloaded.connect(Reloaded)
	health.deathSignal.connect(OnDeath)
	
	detectionArea.scale = Vector2(checkDistance, checkDistance)
	
	detectionArea.area_entered.connect(func(_otherArea) -> void: playerDetected = true)
	detectionArea.area_exited.connect(func(_otherArea) -> void: playerDetected = false)
	
func GetPlayer() -> void:
	player = get_tree().get_first_node_in_group("Player") as CharacterController

func _process(delta: float) -> void:
	
	if reloaded and playerDetected:
		weapon.Fire()
		reloaded = false
		
	

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
	
	var graveInstance = load("res://Gameplay/Enemies/E_Corpse.tscn") as Resource
	
	var deathInstance = graveInstance.instantiate()
	
	deathInstance.global_position = global_position
	
	get_tree().current_scene.add_child(deathInstance)
	queue_free()
	
