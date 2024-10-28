extends Node




@export var maxHealth: int

var currentHealth: int

signal healthChanged(newHealth)
signal death()

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	SetHealth(maxHealth)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func SetHealth(newHealth: int) -> void:
	currentHealth = newHealth
	emit_signal("healthChanged", currentHealth)
	
func Heal(addHealth: int) -> void:
	var newHealth = mini(maxHealth, currentHealth + addHealth)
	SetHealth(newHealth)
	
func Damage(damage: int) -> void:
	SetHealth(currentHealth - damage)
	if currentHealth <= 0:
		emit_signal("death")
