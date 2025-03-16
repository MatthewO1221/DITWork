class_name ControlCurve
extends Resource


enum _ControlStates
{
	Inactive,
	Attack,
	Sustain,
	Release
}

var state : _ControlStates = _ControlStates.Inactive


@export var attackStateDuration : float

@export var releaseStateDuration : float

@export var _minValue : ValueContainer
@export var _maxValue : ValueContainer
var _curValue : Variant

func Ready() -> void:
	_curValue = _minValue.GetValue()
	
func process(delta: float) -> void:
	match state:
		_ControlStates.Inactive:
			return
		_ControlStates.Attack:
			Attack(delta)
		_ControlStates.Sustain:
			return
		_ControlStates.Release:
			Release(delta)
	

func Attack(delta: float) -> void:
	var stepRate = (_maxValue.GetValue() - _minValue.GetValue()) / attackStateDuration
	
	_curValue += stepRate * delta
	
	if _curValue >= _maxValue.GetValue():
		_curValue = _maxValue.GetValue()
		state = _ControlStates.Sustain
	
func Release(delta: float) -> void:
	var stepRate = (_maxValue.GetValue() - _minValue.GetValue()) / releaseStateDuration
	
	_curValue -= stepRate * delta
	
	if _curValue <= _minValue.GetValue():
		_curValue = _minValue.GetValue()
		state = _ControlStates.Inactive
		
func GetValue() -> Variant:
	return _curValue

func Activate() -> void:
	state = _ControlStates.Attack
	
func Deactivate() -> void:
	state = _ControlStates.Release

func IsActive() -> bool:
	return state != _ControlStates.Inactive
