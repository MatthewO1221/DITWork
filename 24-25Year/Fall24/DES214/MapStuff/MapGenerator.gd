extends Node2D

#reference to tilemap object
@export_group("Debug Values")
@export var tileMap: TileMap
@export var timeToWait: float
@export var camera: Camera2D
@export var zoomOut: int
@export var zoomIn: int
@export_group("")

#tiletype enum
enum TileType {Floor = 1, Wall = 2}

#time to wait for slowing things down


#Maps the directions to numbers to make random generation easier
var directionMap = {
	0: Directions.N,
	1: Directions.E,
	2: Directions.S,
	3: Directions.W
}

#The four directions to choose from, mapped to the values used by the tileset
enum Directions {N = TileSet.CELL_NEIGHBOR_TOP_SIDE, 
E = TileSet.CELL_NEIGHBOR_RIGHT_SIDE,
S = TileSet.CELL_NEIGHBOR_BOTTOM_SIDE,
W = TileSet.CELL_NEIGHBOR_LEFT_SIDE}



#Adjustable variables

@export_group("Generation Values") 
@export var maxRoomSize: int
@export var minRoomSize: int
@export var maxCorridorLength: int
@export var minCorridorLength: int
@export var mapSize: int
@export var branchChance: float
@export var roomChance: float
@export_group("")

#Important Globals
var stillGenerating = true
var currentTile = Vector2i(0,0)
var currentTimer: float
var branchList: Array

func Generate() -> void:
	
		
		
	var placeTile = false
	for i in range(10):
		var dir = randi_range(0,3)
		if IsValidNeighbor((currentTile), directionMap[dir]):
			var roomMaybe = randf_range(0, 1.0)
			if roomMaybe < roomChance:
				SpawnRoom(currentTile)
				currentTile = branchList.pop_front()
			else:
				currentTile = PlaceCorridor(TileType.Floor, currentTile, dir)
			
			#currentTile = GetCoordsInDir(currentTile, directionMap[dir])
			placeTile = true
			break
			
	if !placeTile || !currentTile || currentTile.x > mapSize || currentTile.x < -mapSize || currentTile.y > mapSize || currentTile.y < -mapSize:
		if branchList.is_empty():
			stillGenerating = false
		else:
			currentTile = branchList.pop_front()
		
		

		
func PlaceTile(type: TileType, coords: Vector2i) -> void:
	tileMap.set_cell(0, coords, 0, Vector2i(0,0), type)
	var rando = randf_range(0, 1.0)
	if rando < branchChance:
		branchList.push_front(coords)
	
	
func PlaceTileInDirection(type: TileType, coords:Vector2i, dir: Directions) -> void:
	PlaceTile(TileType.Floor, GetCoordsInDir(coords, dir))
	
func PlaceCorridor(type: TileType, coords: Vector2i, dir: Directions) -> Vector2:
	var length = randi_range(minCorridorLength, maxCorridorLength)
	for i in range(length):
		if IsValidNeighbor(coords, directionMap[dir]):
			PlaceTileInDirection(TileType.Floor, coords, directionMap[dir])
			coords = GetCoordsInDir(coords, directionMap[dir])
	
	return coords
	

func IsValidNeighbor(parent: Vector2i, dir: TileSet.CellNeighbor) -> bool:
	var newTile = tileMap.get_neighbor_cell(parent, dir)
	
	for i in range(4):
		if !IsValidPlacement(tileMap.get_neighbor_cell(newTile, directionMap[i])) && tileMap.get_neighbor_cell(newTile, directionMap[i]) != parent:
			return false
	
	return true

func IsValidPlacement(spot: Vector2i) -> bool:
	var id = tileMap.get_cell_source_id(0, spot)
	
	if id == -1:
		return true
	else:
		return false

func GetCoordsInDir(coords: Vector2i, dir: Directions) -> Vector2i:
	var returnDir = coords
	
	if dir == Directions.N:
		returnDir.y -= 1
	elif dir == Directions.E:
		returnDir.x += 1
	elif dir == Directions.S:
		returnDir.y += 1
	else:
		returnDir.x -= 1
		
	return returnDir
	
#region Room Code
	
func SpawnRoom(coords: Vector2i) -> void:
	var roomToPlace = ChooseRoom(coords)
	
	if roomToPlace != null:
		tileMap.set_pattern(0, coords, roomToPlace)
	
func CheckRoom(coords: Vector2i, room: int) -> bool:
	
	var roomToCheck = tileMap.tile_set.get_pattern(room)
	var roomTiles = roomToCheck.get_used_cells()
	
	for i in roomTiles:
		var curTile = tileMap.map_pattern(coords, i, roomToCheck)
		if tileMap.get_cell_source_id(0, curTile) != -1:
			return false
			
	return true
	
func ChooseRoom(coords: Vector2i) -> TileMapPattern:
	var patternsCount = tileMap.tile_set.get_patterns_count()
	for i in range(10):
		var randRoom = randi_range(0, patternsCount - 1)
		if CheckRoom(coords, randRoom):
			return tileMap.tile_set.get_pattern(randRoom)
	
	return tileMap.tile_set.get_pattern(0)

#endregion
	
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	PlaceTile(TileType.Floor, currentTile)
	currentTimer = timeToWait
	camera.zoom = Vector2(zoomIn, zoomIn)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	currentTimer -= delta
	
	if currentTimer > 0:
		return
	
	
	currentTimer = timeToWait
	
	if stillGenerating:
		Generate()

func _input(event: InputEvent) -> void:
	if InputMap.event_is_action(event, "Reset") && event.is_action_pressed("Reset"):
		tileMap.clear_layer(0)
		stillGenerating = true
		currentTile = Vector2i(0,0)
		branchList.clear()
	if InputMap.event_is_action(event, "Zoom") && event.is_action_pressed("Zoom"):
		if camera.zoom.x == zoomIn:
			camera.zoom = Vector2(zoomOut, zoomOut)
		else:
			camera.zoom = Vector2(zoomIn, zoomIn)
		

	
	
