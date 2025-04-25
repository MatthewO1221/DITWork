class_name CustomCurve
extends Resource






## Uses built in Godot tween
var curveType : Tween.TransitionType

## Uses built in Godot tween
var easeType : Tween.EaseType

## The value we're starting at
var begin : Variant

## The value we're going to
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

## Returns the value at the current amount of time passed
func GetValue(timePassed : float, duration: float) -> Variant:
	# This logic is necessary because of a fundemental Godot engine bug where Tweens past their end point will simply return null instead of extrapolating, took a very long time to find this bug
	if timePassed > duration:
		return end
	return Tween.interpolate_value(begin, end - begin, timePassed, duration, curveType, easeType)
	
## Can be used for manual value finding
func GetValueFrom(start: Variant, finish: Variant, timePassed: float, duration: float) -> Variant:
	return Tween.interpolate_value(start, finish - start, timePassed, duration, curveType, easeType)
