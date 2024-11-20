class_name PlayerWeaponBase
extends WeaponBase


func _ready() -> void:
	super()
	
	TelemetrySystem.connect_signal(Firing, func(_a,_b, lambda: Callable) -> void: lambda.call(), "PlayerShotsFired")
	
func _input(event: InputEvent) -> void:
	if event.is_action("Fire") and event.is_action_pressed("Fire"):
		if reloadCountdown <= 0.0:
			var mousePos = get_global_mouse_position()
			reloadCountdown = reloadTime
			match firingMode:
				FiringModes.Burst:
					FireBurst(mousePos)
				FiringModes.Shotgun:
					FireShotgun(mousePos)
				FiringModes.Spread:
					FireSpread(mousePos)
