class_name Hurtbox
extends Component

@export var damage : float = 10.0
@export var knockbackStrength : float = 0.0

@export_category("DoT Values")
@export var damagesOverTime : bool = false
@export var damageDuration : float = 0.0


signal collided(other: Node2D)

func OnBodyCollision(body: Node2D) -> void:
	if body is Hitbox:
		var hitbox := body as Hitbox
		DealDamage(hitbox)
		ApplyKnockback(hitbox)
		collided.emit(body)



func DealDamage(hitbox: Hitbox) -> void:
	hitbox.TakeDamage(damage)

func ApplyKnockback(hitbox: Hitbox) -> void:
	var parent := hitbox.parent
	
	var collisionNormal := (parent.global_position - global_position).normalized()
	
	if parent is RigidBody2D:
		var rb := parent as RigidBody2D
		
		rb.apply_impulse(collisionNormal * knockbackStrength)
	elif parent is CharacterBody2D:
		var cb := parent as CharacterBody2D
		
		cb.velocity += collisionNormal * knockbackStrength

func ReceiveData(data: Dictionary[String, Variant]) -> void:
	var propertyList := self.get_property_list()
	for key in data.keys():
		for property in propertyList:
			var propertyName := property.name as String
			if propertyName.to_lower() == key.to_lower():
				assert(property.type == typeof(data[key]), "Attempted to set variable with wrong data type")
				self.set(propertyName, data[key])
