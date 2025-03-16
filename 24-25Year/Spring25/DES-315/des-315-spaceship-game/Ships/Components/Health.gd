class_name Health
extends Node



signal HealthDepleted

@export var maxHealth : int = 100


var curHealth : int


func _ready() -> void:
	curHealth = maxHealth
	
func Damage(damage: int) -> void:
	curHealth -= damage
	
	if curHealth <= 0:
		HealthDepleted.emit()
