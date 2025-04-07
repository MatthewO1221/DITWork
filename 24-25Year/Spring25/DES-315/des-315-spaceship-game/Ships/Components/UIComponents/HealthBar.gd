class_name HealthBar
extends Component



func _ready() -> void:
	super()
	
	var hitbox = parent as Hitbox
	
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
