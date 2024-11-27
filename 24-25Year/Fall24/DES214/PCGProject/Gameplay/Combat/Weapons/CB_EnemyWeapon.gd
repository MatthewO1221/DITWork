class_name EnemyWeaponBase
extends WeaponBase


var player: Node

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	super()
	GetPlayer.call_deferred()
	
	
	TelemetrySystem.connect_signal(Firing, func(_a,_b, lambda: Callable) -> void: lambda.call(),get_parent().enemyName + "ShotsFired")

func GetPlayer() -> void:
	player = get_tree().get_nodes_in_group("Player")[0] if get_tree().has_group("Player") else null

func Fire() -> void:
	if reloadCountdown <= 0.0:
		match firingMode:
			FiringModes.Burst:
				FireBurst(player.global_position)
			FiringModes.Shotgun:
				FireShotgun(player.global_position)
			FiringModes.Spread:
				FireSpread(player.global_position)
		reloadCountdown = reloadTime
