class_name WeaponBase
extends Node


enum FiringModes
{
	Burst,
	Spread,
	Shotgun
}

@export_category("Weapon Variables")
@export var bullet: Node
@export var reloadTime := 1.0
@export var numBullets := 1
@export var firingMode: FiringModes

@export_category("Bullet Variables")
@export var bulletDamage := 1
@export var bulletSize := 1.0
@export var bulletSpeed := 1.0
@export var bulletRange := 10.0


var reloadCountdown: float

signal Firing(shooter: Node, weaponType: FiringModes)
signal Reloaded

func Fire() -> void:
	pass
	
func _ready() -> void:
	reloadCountdown = reloadTime

func _process(delta: float) -> void:
	reloadCountdown -= delta
	
	if reloadCountdown <= 0.0:
		emit_signal("Reloaded")
