class_name HealthComponent
extends Node2D


@export var maxHealth := 100

var hurtbox: HurtboxComponent

var curHealth := 0


signal damageTaken(damage: int, source: Node)
signal healed(healing: int, source: Node)
signal healthChanged(newHealth: int)
signal deathSignal(killer: Node)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	curHealth = maxHealth
	healthChanged.emit(curHealth)
	
	var parent = get_parent()
	
	var hurtbox = parent.get_node(parent.get_meta("Hurtbox")) as HurtboxComponent
	
	
	
	hurtbox.collided.connect(ResolveCollision)
	
	TelemetrySystem.connect_signal(damageTaken, func(_a,_b, lambda: Callable) -> void: lambda.call(), get_parent().get_class() + "DamageTaken")
	TelemetrySystem.connect_signal(healed, func(_a,_b, lambda: Callable) -> void: lambda.call(), get_parent().get_class() + "Healed")
	TelemetrySystem.connect_signal(deathSignal, func(_a, lambda: Callable) -> void: lambda.call(), get_parent().get_class())


func ResolveCollision(hurtbox: HurtboxComponent, source: Node) -> void:
	var damage = hurtbox.damage
	
	if damage > 0:
		TakeDamage(damage, source)
	elif damage < 0:
		Heal(-damage, source)
	
func TakeDamage(damage: int, source: Node) -> void:
	curHealth -= damage
	
	damageTaken.emit( damage, source)
	healthChanged.emit(curHealth)
	
	if curHealth <= 0:
		deathSignal.emit(source)

func Heal(healing: int, source: Node) -> void:
	curHealth += healing
	healed.emit(healing, source)
	healthChanged.emit(curHealth)
