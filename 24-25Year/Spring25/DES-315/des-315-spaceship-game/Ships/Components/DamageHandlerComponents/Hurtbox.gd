class_name Hurtbox
extends Component

@export var damage : float = 10.0
@export var knockbackStrength : float = 0.0

@export_category("DoT Values")
@export var damagesOverTime : bool = false
@export var damageDuration : float = 0.0


func OnCollision(body: Node2D) -> void:
	if body is Hitbox:
		var hitbox := body as Hitbox
		DealDamage(hitbox)

func DealDamage(hitbox: Hitbox) -> void:
	hitbox.TakeDamage(damage)
