class_name Hitbox
extends Component


@export var maxHealth : float = 100.0

@export var regenerates : bool = false

@export var regenerationRate : float = 0.0


var currentHealth : float

signal HealthChanged(newHealth: float)
signal DamageTaken(damage: float)
signal HealthDepleted
signal Healed(health: float)

func _ready() -> void:
	super()
	currentHealth = maxHealth
	
func _process(delta: float) -> void:
	if regenerates and currentHealth < maxHealth:
		currentHealth += regenerationRate * maxHealth * delta
		

	
func TakeDamage(damage : float) -> void:
	currentHealth -= damage
	DamageTaken.emit(damage)
	
	if currentHealth <= 0.0:
		HealthDepleted.emit()
	
func Heal(health: float) -> void:
	currentHealth += health
	Healed.emit(health)
