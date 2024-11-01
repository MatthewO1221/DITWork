class_name PlayerWeaponBase
extends WeaponBase


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
	
func _input(event: InputEvent) -> void:
	if event.is_action("Fire") and event.is_action_pressed("Fire"):
		if reloadCountdown <= 0.0:
			var mousePos = get_global_mouse_position()
			match firingMode:
				FiringModes.Burst:
					FireBurst(mousePos)
				FiringModes.Shotgun:
					FireShotgun(mousePos)
				FiringModes.Spread:
					FireSpread(mousePos)
