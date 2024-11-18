extends Control

# Dictionary to store telemetry data
var telemetry_data: Dictionary = {}

# File path for saving telemetry logs
const TELEMETRY_FILE_PATH: String = "res://telemetry_log.csv"

# Initializes a counter if it doesn't exist
func ensure_counter(counter_name: String, initial_value: int = 0):
	if not telemetry_data.has(counter_name):
		telemetry_data[counter_name] = initial_value

# Increment a counter
func increment_counter(counter_name: String, amount: int = 1):
	ensure_counter(counter_name)
	telemetry_data[counter_name] += amount

# Get the value of a counter
func get_counter(counter_name: String) -> int:
	return telemetry_data.get(counter_name, 0)

# Reset a counter
func reset_counter(counter_name: String):
	if telemetry_data.has(counter_name):
		telemetry_data[counter_name] = 0

# Export telemetry data to a CSV file
func export_to_csv():
	var file = FileAccess.open(TELEMETRY_FILE_PATH, FileAccess.WRITE)
	if file.is_open() == true:
		# Write headers
		file.store_line("Metric,Value")
		# Write each telemetry data item
		for key in telemetry_data.keys():
			file.store_line("%s,%d" % [key, telemetry_data[key]])
		file.close()
		print("Telemetry exported to %s" % TELEMETRY_FILE_PATH)
	else:
		print("Failed to open file for telemetry export!")

# Connect the manager to various signals
func connect_signal(emitter: Object, signal_: String, counter_name: String, increment_value: int = 1) -> void:
	# Prepare the data dictionary to pass to the signal
	var data = {
		"counter_name": counter_name,
		"increment_value": increment_value
	}
	# Connect the signal, passing the data dictionary
	if not emitter.is_connected(signal_, _on_signal_emitted):
		emitter.connect(signal_, _on_signal_emitted.bind(data))

# Handle connected signals
func _on_signal_emitted(data: Dictionary):
	if data.has("counter_name") and data.has("increment_value"):
		increment_counter(data["counter_name"], data["increment_value"])

# Log telemetry data for debugging
func log_telemetry():
	for key in telemetry_data.keys():
		print("%s: %d" % [key, telemetry_data[key]])

# Reset all telemetry data
func reset_all():
	telemetry_data.clear()
	
func _notification(what):
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		
		print(is_connected("Firing", _on_signal_emitted))
		export_to_csv()
