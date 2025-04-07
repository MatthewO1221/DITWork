class_name Ammunition
extends RigidBody2D

@export_category("Ammunition Variables")
@export var damage: float = 0.0
@export var knockbackStrength: float = 0.0

func ReceiveData(data: Dictionary[String, Variant]) -> void:
	var propertyList := self.get_property_list()
	for key in data.keys():
		for property in propertyList:
			var propertyName := property.name as String
			if propertyName.to_lower() == key.to_lower():
				assert(property.type == typeof(data[key]), "Attempted to set variable with wrong data type")
				self.set(propertyName, data[key])
		
