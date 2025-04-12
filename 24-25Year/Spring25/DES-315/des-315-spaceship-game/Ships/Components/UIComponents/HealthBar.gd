class_name HealthBar
extends Control



func _ready() -> void:
	
	var hitbox
	
	hitbox = get_parent()
	
	while hitbox is not Hitbox:
		hitbox = hitbox.get_parent()
	
	$ProgressBar.max_value = hitbox.maxHealth
	
	hitbox.HealthChanged.connect(UpdateBar)
	
	Settings.showHealthBarsChanged.connect(ShoworHide)

func UpdateBar(newHealth: float) -> void:
	$ProgressBar.value = newHealth

func ShoworHide(shown: bool) -> void:
	if shown:
		$ProgressBar.visible = true
	else:
		$ProgressBar.visible = false
