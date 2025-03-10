class_name CustomCurve
extends Resource







var curveType : Tween.TransitionType
var easeType : Tween.EaseType

var begin : Variant
var end : Variant


func _init(curve: Tween.TransitionType, ease : Tween.EaseType = Tween.EaseType.EASE_IN) -> void:
	curveType = curve
	easeType = ease

	
	


	
	
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func SetStart(start: Variant) -> void:
	begin = start
	
func SetFinish(finish: Variant) -> void:
	
	end = finish

func GetValue(timePassed : float, duration: float) -> Variant:
	if timePassed > duration:
		return end
	return Tween.interpolate_value(begin, end - begin, timePassed, duration, curveType, easeType)
	
func GetValueFrom(start: Variant, finish: Variant, timePassed: float, duration: float) -> Variant:
	return Tween.interpolate_value(start, finish - start, timePassed, duration, curveType, easeType)
