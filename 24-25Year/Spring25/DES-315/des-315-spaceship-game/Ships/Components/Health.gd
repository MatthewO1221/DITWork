class_name Health
extends Node



signal HealthDepleted

signal DamageTaken

@export var maxHealth : float = 100
@export var regenerates : bool = true
@export var regenerationRate : float = 0.05


var curHealth : float



func _ready() -> void:
	curHealth = maxHealth
	
func Damage(damage: int) -> void:
	curHealth -= damage
	
	DamageTaken.emit()
	
	if curHealth <= 0:
		HealthDepleted.emit()
		
func _process(delta: float) -> void:
	if regenerates and curHealth < maxHealth:
		curHealth += maxHealth * regenerationRate * delta
