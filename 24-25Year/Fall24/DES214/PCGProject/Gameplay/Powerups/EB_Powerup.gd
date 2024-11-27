class_name PowerupBase
extends Node2D

enum Types{shotBoost, healthBoost, speedBoost}


var area: Area2D

signal pickupGrabbed(pickupType: Types)

@export var pickupType: Types
@export var boostAmount: int

var playerHealth: HealthComponent
var playerWeapon: PlayerWeaponBase
var playerBody: CharacterController

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	area = $Area2D
	
	area.area_entered.connect(OnPickup)
	
	
	Initialize.call_deferred()

func Initialize() -> void:
	var player = get_tree().get_first_node_in_group("Player") as CharacterController
	
	playerHealth = player.get_node(player.get_meta("Health"))
	playerWeapon = player.get_node(player.get_meta("Weapon"))
	playerBody = player as CharacterController

func OnPickup(otherArea: Area2D) -> void:
	pickupGrabbed.emit(pickupType)
