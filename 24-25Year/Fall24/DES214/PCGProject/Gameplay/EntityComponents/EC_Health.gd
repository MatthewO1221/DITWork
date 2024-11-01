class_name HealthComponent
extends Node


@export var maxHealth := 100

var curHealth := 0


signal damageTaken(damage: int, source: Node)
signal healed(healing: int, source: Node)
signal deathSignal(killer: Node)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	curHealth = maxHealth


	
	
func TakeDamage(damage: int, source: Node) -> void:
	curHealth -= damage
	
	emit_signal("damageTaken", damage, source)
	
	if curHealth <= 0:
		emit_signal("deathSignal", source)

func Heal(healing: int, source: Node) -> void:
	curHealth += healing
	emit_signal("healed", healing, source)
