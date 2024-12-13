class_name Player
extends CharacterBody3D


@export var Speed := 5.0
@export var JumpStrength := 4.5
@export var RotateSpeed := 5.0

var rising := false

var TelemetryData: Dictionary = {}

var eventHappened := 0

var currentZone: String = "Setup"

var minx := INF
var miny := INF
var minz := INF
var maxx := -INF
var maxy := -INF
var maxz := -INF

var maxSpeed := 0.0

var eventNumber := 0


var Keys: Dictionary = {
	#time = "Time",
	#Zone = "Zone",
	#PosX = "PosX",
	#PosY = "PosY",
	#PosZ = "PosZ",
	#VelX = "VelX",
	#VelY = "VelY",
	#VelZ = "VelZ",
	#Events = "Events",
	#TimeinForest = "TimeinForest",
	#TimeinCave = "TimeinCave",
	#TimeinSwamp = "TimeinSwamp",
	#JumpsinForest = "JumpsinForest",
	#JumpsinCave = "JumpsinCave",
	#JumpsinSwamp = "JumpsinSwamp",
	#DistanceTraveledinForest = "DistanceTraveledinForest",
	#DistanceTraveledinCave = "DistanceTraveledinCave",
	#DistanceTraveledinSwamp = "DistanceTraveledinSwamp",
	#MaxSpeed = "MaxSpeed",
	#TotalJumps = "TotalJumps",
	#MinX = "MinX",
	#MinY = "MinY",
	#MinZ = "MinZ",
	#MaxX = "MaxX",
	#MaxY = "MaxY",
	#MaxZ = "MaxZ",
	#TotalDistanceTraveled = "TotalDistanceTraveled",
}

var startTime: float

var forestStartTime: float
var caveStartTime: float
var swampStartTime: float

func _ready() -> void:
	startTime = Time.get_unix_time_from_system()
	InitKeys()
	InitValues()

func _process(delta: float) -> void:
	LogValue("Time", Time.get_unix_time_from_system() - startTime)
	LogValue("Zone", currentZone)
	LogValue("PosX", global_position.x)
	LogValue("PosY", global_position.y)
	LogValue("PosZ", global_position.z)
	LogValue("VelX", velocity.x)
	LogValue("VelY", velocity.y)
	LogValue("VelZ", velocity.z)
	IncrementCounter("TotalDistanceTraveled", velocity.length())
	if currentZone != "Setup" and currentZone != "Resolution":
		IncrementCounter("DistanceTraveledin" + currentZone, velocity.length())
		
	LogValue("Events", eventHappened)
	eventHappened = 0
	
	if velocity.length() > maxSpeed:
		maxSpeed = velocity.length()

func _physics_process(delta: float) -> void:
	# Add the gravity.
	if not is_on_floor() and !rising:
		velocity += get_gravity() * delta
	
	
	
	
	if global_position.x < minx:
		minx = global_position.x
	if global_position.y < miny:
		miny = global_position.y
	if global_position.z < minz:
		minz = global_position.z
		
	if global_position.x > maxx:
		maxx = global_position.x
	if global_position.y > maxy:
		maxy = global_position.y
	if global_position.z > maxz:
		maxz = global_position.z
	
	# Handle jump.
	if Input.is_action_just_pressed("Jump") and is_on_floor() and !rising:
		velocity.y = JumpStrength
		if currentZone != "Setup" and currentZone != "Resolution":
			IncrementCounter("Jumpsin" + currentZone, 1)
		IncrementCounter("TotalJumps", 1)
		LogValue("Jumps", 1)
	else:
		LogValue("Jumps", 0)

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var input_dir := Input.get_vector("Left", "Right", "Forward", "Backward")
	var direction := (transform.basis * Vector3(input_dir.x, 0, input_dir.y)).normalized()
	if direction:
		velocity.x = direction.x * Speed
		velocity.z = direction.z * Speed
	else:
		velocity.x = move_toward(velocity.x, 0, Speed)
		velocity.z = move_toward(velocity.z, 0, Speed)
		
	if Input.is_action_pressed("RotateLeft") and !Input.is_action_pressed("RotateRight"):
		rotation.y += RotateSpeed * delta
	elif Input.is_action_pressed("RotateRight") and !Input.is_action_pressed("RotateLeft"):
		rotation.y -= RotateSpeed * delta
	
	
	
	
	
	
	
	
	
	move_and_slide()

func InitKeys() -> void:
	TelemetryData.get_or_add("Time")
	TelemetryData.get_or_add("Zone")
	TelemetryData.get_or_add("PosX")
	TelemetryData.get_or_add("PosY")
	TelemetryData.get_or_add("PosZ")
	TelemetryData.get_or_add("VelX")
	TelemetryData.get_or_add("VelY")
	TelemetryData.get_or_add("VelZ")
	TelemetryData.get_or_add("Events")
	TelemetryData.get_or_add("Jumps")
	TelemetryData.get_or_add("TimeinForest")
	TelemetryData.get_or_add("TimeinCave")
	TelemetryData.get_or_add("TimeinSwamp")
	TelemetryData.get_or_add("JumpsinForest")
	TelemetryData.get_or_add("JumpsinCave")
	TelemetryData.get_or_add("JumpsinSwamp")
	TelemetryData.get_or_add("DistanceTraveledinForest")
	TelemetryData.get_or_add("DistanceTraveledinCave")
	TelemetryData.get_or_add("DistanceTraveledinSwamp")
	TelemetryData.get_or_add("MaxSpeed")
	TelemetryData.get_or_add("TotalJumps")
	TelemetryData.get_or_add("MinX")
	TelemetryData.get_or_add("MinY")
	TelemetryData.get_or_add("MinZ")
	TelemetryData.get_or_add("MaxX")
	TelemetryData.get_or_add("MaxY")
	TelemetryData.get_or_add("MaxZ")
	TelemetryData.get_or_add("TotalDistanceTraveled")

func InitValues() -> void:
	TelemetryData["Time"] = Array()
	TelemetryData["Zone"] = Array()
	TelemetryData["PosX"] = Array()
	TelemetryData["PosY"] = Array()
	TelemetryData["PosZ"] = Array()
	TelemetryData["VelX"] = Array()
	TelemetryData["VelY"] = Array()
	TelemetryData["VelZ"] = Array()
	TelemetryData["Events"] = Array()
	TelemetryData["Jumps"] = Array()
	TelemetryData["TimeinForest"] = 0.0
	TelemetryData["TimeinCave"] = 0.0
	TelemetryData["TimeinSwamp"] = 0.0
	TelemetryData["JumpsinForest"] = 0
	TelemetryData["JumpsinCave"] = 0
	TelemetryData["JumpsinSwamp"] = 0
	TelemetryData["DistanceTraveledinForest"] = 0.0
	TelemetryData["DistanceTraveledinCave"] = 0.0
	TelemetryData["DistanceTraveledinSwamp"] = 0.0
	TelemetryData["MaxSpeed"] = 0.0
	TelemetryData["TotalJumps"] = 0
	TelemetryData["MinX"] = 0.0
	TelemetryData["MinY"] = 0.0
	TelemetryData["MinZ"] = 0.0
	TelemetryData["MaxX"] = 0.0
	TelemetryData["MaxY"] = 0.0
	TelemetryData["MaxZ"] = 0.0
	TelemetryData["TotalDistanceTraveled"] = 0.0

func IncrementCounter(key: String, amount: Variant) -> void:
	assert(TelemetryData.has(key), "TelemetryData does not have key: " + key)
	TelemetryData[key] += amount
	
func DecrementCounter(key: String, amount: Variant) -> void:
	assert(TelemetryData.has(key), "TelemetryData does not have key: " + key)
	TelemetryData[key] -= amount

func LogValue(key: String, value: Variant) -> void:
	assert(TelemetryData.has(key), "TelemetryData does not have key: " + key)
	var telemetryArray = TelemetryData[key] as Array
	
	telemetryArray.append(value)
	
	TelemetryData[key] = telemetryArray


func ChangeZone(newZone: String) -> void:
	currentZone = newZone
	
	match newZone:
		"Forest":
			forestStartTime = Time.get_unix_time_from_system()
		"Cave":
			caveStartTime = Time.get_unix_time_from_system()
			IncrementCounter("TimeinForest", Time.get_unix_time_from_system() - forestStartTime)
		"Swamp":
			swampStartTime = Time.get_unix_time_from_system()
			IncrementCounter("TimeinCave", Time.get_unix_time_from_system() - caveStartTime)
		"Resolution":
			IncrementCounter("TimeinSwamp", Time.get_unix_time_from_system() - caveStartTime)

func LogEvent() -> void:
	eventHappened = 1

func _exit_tree() -> void:
	IncrementCounter("MaxSpeed", maxSpeed)
	IncrementCounter("MinX", minx)
	IncrementCounter("MinY", miny)
	IncrementCounter("MinZ", minz)
	IncrementCounter("MaxX", maxx)
	IncrementCounter("MaxY", maxy)
	IncrementCounter("MaxZ", maxz)
	
	ExportData()

func ExportData() -> void:
	# Get the user's Documents directory
	var documentsPath = OS.get_system_dir(OS.SYSTEM_DIR_DOCUMENTS)
	var customFolder = "WalkingSimTelemetry"
	var fullPath = documentsPath + "/" + customFolder
	
	DirAccess.make_dir_absolute(fullPath)
	var dir = DirAccess.open(fullPath)
	if not dir.dir_exists(fullPath):
		dir.make_dir_recursive(fullPath)
	
	var timeDateDict = Time.get_datetime_dict_from_system()
	var filePath = fullPath + "/" + "TelemetryData" + "_" + str(timeDateDict.hour) + "_" + str(timeDateDict.minute) + "_" + str(timeDateDict.second) + ".csv"
	
	var file = FileAccess.open(filePath, FileAccess.WRITE)
	
	assert(file.is_open(), "Error opening telemetry file")
	
	var headers: PackedStringArray
	
	for key in TelemetryData.keys():
		headers.append(key)
	
	file.store_csv_line(headers)
	
	var firstData: PackedStringArray
	
	for key in TelemetryData.keys():
		if typeof(TelemetryData[key]) == TYPE_ARRAY:
			firstData.append(str(TelemetryData[key][0]))
		else:
			firstData.append(str(TelemetryData[key]))
	
	file.store_csv_line(firstData)
	
	for i in range(1, TelemetryData["Time"].size()):
		var outputData: PackedStringArray
		for key in TelemetryData.keys():
			if typeof(TelemetryData[key]) == TYPE_ARRAY:
				outputData.append(str(TelemetryData[key][i]))
		file.store_csv_line(outputData)
	file.close()
	print("Telemetry exported to %s" % filePath)
