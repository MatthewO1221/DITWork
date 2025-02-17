class_name CustomCurve
extends Node







var curveType : Tween.TransitionType
var easeType : Tween.EaseType

var begin : Variant
var end : Variant


func _init(start: Variant, finish: Variant, curve: Tween.TransitionType, ease : Tween.EaseType = Tween.EaseType.EASE_IN) -> void:
	curveType = curve
	easeType = ease
	assert(typeof(start) == typeof(finish), "Start and Finish are different value types")
	
	begin = start
	end = finish
	


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass



func GetValue(timePassed : float, duration: float) -> Variant:
	return Tween.interpolate_value(begin, end - begin, timePassed, duration, curveType, easeType)
