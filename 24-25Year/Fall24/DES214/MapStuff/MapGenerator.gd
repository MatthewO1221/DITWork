#TODO
#Make room placement shift to entrance tile
#Make rooms rotate to place properly
#Make some interesting varied rooms
#Make interesting corridor rooms
#Create Biome Mode Structs


#Plan
#Step 1: Generate Rooms Randomly Throughout Level
#Step 2: Add all entrance tiles to branch list
#Step 3: Let algorithm generate things from branches



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
enum TileType {Empty = -1, Floor = 1, Wall = 2, Entrance = 3}


enum DungeonRooms
{
	Basic = 1
}

enum DensityModes
{
	Sparse,
	Regular,
	Cluttered
}


class Mode:
	var name: String
	var roomList: Dictionary
	var roomAmount: DensityModes
	var corridorList: Dictionary
	var corridorAmount: DensityModes
	var enemyList: Dictionary
	var lootList: Dictionary
	
	var minCorridorLength: int
	var maxCorridorLength: int
	var branchChance: float


class Region:
	var name: String
	var totalTiles: int
	var area: Rect2i


#Maps the directions to numbers to make random generation easier
var directionMap = {
	0: Directions.N,
	1: Directions.E,
	2: Directions.S,
	3: Directions.W
}

enum TileTransform {
	ROTATE_0 = 0,
	ROTATE_90 = TileSetAtlasSource.TRANSFORM_TRANSPOSE | TileSetAtlasSource.TRANSFORM_FLIP_H,
	ROTATE_180 = TileSetAtlasSource.TRANSFORM_FLIP_V | TileSetAtlasSource.TRANSFORM_FLIP_H,
	ROTATE_270 = TileSetAtlasSource.TRANSFORM_FLIP_V | TileSetAtlasSource.TRANSFORM_TRANSPOSE,
}

#The four directions to choose from, mapped to the values used by the tileset
enum Directions {N = TileSet.CELL_NEIGHBOR_TOP_SIDE, 
E = TileSet.CELL_NEIGHBOR_RIGHT_SIDE,
S = TileSet.CELL_NEIGHBOR_BOTTOM_SIDE,
W = TileSet.CELL_NEIGHBOR_LEFT_SIDE}

class Room:
	var Index: int
	var Offset: Vector2i
	var Rotation: TileTransform
	



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
var currentTile = Vector2i(0,0)
var branchList: Array
var generatingCorridor = false


var roomTiles: Array

var corridorTiles: Array

var entranceList: Array

func Generate() -> void:
	
	var testMode = Mode.new()
	testMode.name = "TestMode"
	testMode.roomList = {room1 = 0, room2 = 1, room3 = 2, room4 = 3, room5 = 4}
	testMode.roomAmount = DensityModes.Cluttered
	testMode.corridorAmount = DensityModes.Cluttered
	testMode.minCorridorLength = 5
	testMode.maxCorridorLength = 10
	testMode.branchChance = 0.05
	
	var testRegion = Region.new()
	testRegion.name = "TestRegion"
	testRegion.totalTiles = mapSize ** 2
	testRegion.area = Rect2i(-mapSize, -mapSize, mapSize * 2, mapSize * 2)
	
	#Place rooms in the level
	SpawnRooms(testMode, testRegion)
	
	#Replace all entrance tiles with floors and place them in entrance list
	entranceList.append_array(SweepEntrances())
	
	
	OrganizeEntrances()
	
	ConnectEntrances(testMode, testRegion)
	
	CreateBranches(testMode, testRegion)
		
	
	
	
	#var placeTile = false
	#for i in range(10):
		#var dir = randi_range(0,3)
		#if IsValidNeighbor((currentTile), directionMap[dir]):
			#var roomMaybe = randf_range(0, 1.0)
			#if roomMaybe < roomChance:
				#SpawnRoom(currentTile)
				#currentTile = branchList.pop_front()
			#else:
				#currentTile = PlaceCorridor(TileType.Floor, currentTile, dir)
			#
			##currentTile = GetCoordsInDir(currentTile, directionMap[dir])
			#placeTile = true
			#break
			#
	#if !placeTile || !currentTile || currentTile.x > mapSize || currentTile.x < -mapSize || currentTile.y > mapSize || currentTile.y < -mapSize:
		#if branchList.is_empty():
			#stillGenerating = false
		#else:
			#currentTile = branchList.pop_front()
		
		


func ConnectEntrances(mode: Mode, region: Region) -> void:
	
	var aStar = AStarGrid2D.new()
	
	aStar.region = region.area
	aStar.cell_size = tileMap.tile_set.tile_size
	
	aStar.default_compute_heuristic = AStarGrid2D.HEURISTIC_MANHATTAN
	aStar.default_estimate_heuristic = AStarGrid2D.HEURISTIC_MANHATTAN
	aStar.diagonal_mode = AStarGrid2D.DIAGONAL_MODE_NEVER
	
	aStar.update()
	
	for tile in roomTiles:
		aStar.set_point_solid(tile, true)
		
	for entrance in entranceList:
		aStar.set_point_solid(entrance, false)
	
	
		
	#for tile in allTiles:
		#var neighbors = [Vector2i(1, 0), Vector2i(-1, 0), Vector2i(0, 1), Vector2i(0, -1)]
		#for neighbor in neighbors:
			#var neighbor_pos = tile + neighbor
			#if aStar.has_point(neighbor_pos) and not aStar.is_point_solid(neighbor_pos):
				#aStar.connect_points(tile, neighbor_pos)
		
	
	
	for i in range(entranceList.size()):
		if i == entranceList.size() - 1:
			break
		ConnectTwoEntrances(mode, entranceList[i], entranceList[i + 1] ,aStar)
		
		
func ConnectTwoEntrances(mode: Mode, tile: Vector2i, nextEntrance: Vector2i, grid: AStarGrid2D) -> void:
		
		
		
		if nextEntrance == null:
			return
		
		var path = grid.get_id_path(tile, nextEntrance)
		
		for tiles in path:
			PlaceTile(mode, tiles)
			
		
func CreateBranches(mode: Mode, region: Region) -> void:
	
	while !branchList.is_empty():
		var branch = branchList.pop_front()
		SpawnBranch(branch, mode, region)
		
		match mode.corridorAmount:
			DensityModes.Sparse when float(corridorTiles.size())/region.totalTiles >= 0.25:
				break
			DensityModes.Regular when float(corridorTiles.size())/region.totalTiles >= 0.5:
				break
			DensityModes.Cluttered when float(corridorTiles.size())/region.totalTiles >= 0.75:
				break
		

func SpawnBranch(branch: Vector2i, mode: Mode, region: Region) -> void:
	var currentTile = branch
	
	for i in range(10):
		var dir = randi_range(0,3)
		
		var length = randi_range(mode.minCorridorLength, mode.maxCorridorLength)
		
		for j in range(length):
			if IsValidNeighbor(currentTile, directionMap[dir]):
				currentTile = PlaceTileInDirection(mode, currentTile, directionMap[dir])

func GenerateCorridor(mode: Mode) -> void:
	var placeCorridor = false
	
	for i in range(10):
		var dir = randi_range(0,3)
		
		if IsValidNeighbor(currentTile, directionMap[dir]):
			currentTile = PlaceCorridor(mode, currentTile, dir)
			placeCorridor = true
			break
			
	
	if !placeCorridor:
		generatingCorridor = false
	
	
func SweepEntrances() -> Array[Vector2i]:
	var allTiles = tileMap.get_used_cells_by_id(0, 0, Vector2i(-1,-1), 3)
	
	for tile in allTiles:
		tileMap.set_cell(0, tile, 3, Vector2i(1,1))
	
	return allTiles
		
		
func OrganizeEntrances() -> void:
	var newList = entranceList
	
	newList.sort_custom(OrganizeTiles)
		
	entranceList = newList
		

func OrganizeTiles(t1: Vector2i, t2: Vector2i) -> bool:
	if t1.x < t2.x:
		return true
	elif t1.x == t2.x and t1.y > t2.y:
		return true
	
	
	return false

func SpawnRooms(mode: Mode, region: Region) -> void:
	var roomList = mode.roomList
	var roomDensity = mode.roomAmount
	var totalTiles = region.totalTiles
	var tilesPlaced = 0
	
	while true:
		
		var placeTile = GetRandomTile(region)
		
		for i in range(10):
			var roomToTry = GetRandomRoom(mode)
			var rotation = TileTransform.values().pick_random()
			var canBePlaced = CheckRoom(placeTile, roomToTry, rotation)
			
			if canBePlaced:
				PlaceRoom(roomToTry, placeTile, rotation)
				var tileCount = roomToTry.get_size().x * roomToTry.get_size().y
				tilesPlaced += tileCount
				
				
				break
		
		
		
		match roomDensity:
			DensityModes.Sparse when float(tilesPlaced)/totalTiles >= 0.1:
				break
			DensityModes.Regular when float(tilesPlaced)/totalTiles >= 0.25:
				break
			DensityModes.Cluttered when float(tilesPlaced)/totalTiles >= 0.5:
				break
	
	
	
	

func GetRandomTile(region: Region) -> Vector2i:
	var returnTile: Vector2i
	
	returnTile.x = randi_range(-mapSize, mapSize)
	returnTile.y = randi_range(-mapSize, mapSize)
	
	return returnTile

func GetRandomRoom(mode: Mode) -> TileMapPattern:
	var patternsCount = mode.roomList.size()
	
	var rand = randi_range(0, patternsCount)
	
	var patternIndex = mode.roomList.values().pick_random()
	
	var pattern = tileMap.tile_set.get_pattern(patternIndex)
	
	return pattern
	
	
func PlaceRoom(room: TileMapPattern, coords: Vector2i, rotation: TileTransform) -> void:
	var placedRoom = RotateRoom(room, rotation)
	
	var tiles = placedRoom.get_used_cells()
	
	for tile in tiles:
		roomTiles.append(tileMap.map_pattern(coords, tile, placedRoom))
	
	tileMap.set_pattern(0, coords, placedRoom)
	
	sleep()
	

func PlaceTile(mode: Mode, coords: Vector2i) -> Vector2i:
	tileMap.set_cell(0, coords, 3, Vector2i(1,1))
	corridorTiles.append(coords)
	var rando = randf_range(0, 1.0)
	if rando < mode.branchChance:
		branchList.push_front(coords)
	sleep()
	
	return coords
	
	
func PlaceTileInDirection(mode: Mode, coords:Vector2i, dir: Directions) -> Vector2i:
	return PlaceTile(mode, GetCoordsInDir(coords, dir))
	
func PlaceCorridor(mode: Mode, coords: Vector2i, dir: Directions) -> Vector2i:
	var length = randi_range(mode.minCorridorLength, mode.maxCorridorLength)
	for i in range(length):
		if IsValidNeighbor(coords, directionMap[dir]):
			PlaceTileInDirection(mode, coords, directionMap[dir])
			coords = GetCoordsInDir(coords, directionMap[dir])
	
	return coords
	

func IsValidNeighbor(parent: Vector2i, dir: TileSet.CellNeighbor) -> bool:
	var newTile = tileMap.get_neighbor_cell(parent, dir)
	
	if !IsValidPlacement(newTile):
		return false
	
	for i in range(4):
		var adjacentTile = tileMap.get_neighbor_cell(newTile, directionMap[i])
		if corridorTiles.has(adjacentTile) and adjacentTile != parent:
			return false
	
	return true

func IsValidPlacement(spot: Vector2i) -> bool:
	var id = tileMap.get_cell_source_id(0, spot)
	var atlasCoords = tileMap.get_cell_atlas_coords(0, spot)
	
	if spot.x > mapSize || spot.x < -mapSize || spot.y > mapSize || spot.y < -mapSize:
		return false
	
	if roomTiles.has(spot):
		return false
	
	
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
	
#func SpawnRoom(coords: Vector2i) -> void:
	#var roomToPlace = ChooseRoom(coords)
	#
	#if roomToPlace != null:
		#tileMap.set_pattern(0, coords + roomToPlace.Offset, tileMap.tile_set.get_pattern(roomToPlace.Index))
	#
	
	
	
func CheckRoom(coords: Vector2i, room: TileMapPattern, rotation: TileTransform) -> bool:
	
	var rotatedRoom = RotateRoom(room, rotation)
	var roomTiles = rotatedRoom.get_used_cells()
	var foundPlacement = false
	
	
	
	
	
	for i in roomTiles:
		var curTile = tileMap.map_pattern(coords, i, rotatedRoom)
		if tileMap.get_cell_source_id(0, curTile) != TileType.Empty || curTile.x >= mapSize || curTile.x <= -mapSize || curTile.y >= mapSize || curTile.y <= - mapSize:
			return false
		
	return true
	
	
	
func RotateRoom(pattern: TileMapPattern, rotation: TileTransform) -> TileMapPattern:
	
	if rotation == TileTransform.ROTATE_0:
		return pattern
	
	var newPattern := TileMapPattern.new()
	
	
	var size = pattern.get_size()
	
	for y in range(size.y):
		for x in range(size.x):
			var sourceCoordinates = Vector2i(x, y)
			
			if !pattern.has_cell(sourceCoordinates):
				continue
			
			var newCoords: Vector2i
			
			
			match rotation:
				TileTransform.ROTATE_90:
					newCoords = Vector2i(size.y - y - 1, x)
				TileTransform.ROTATE_180:
					newCoords = Vector2i(size.x - x - 1, size.y - y - 1)
				TileTransform.ROTATE_270:
					newCoords = Vector2i(y, size.x - x - 1)
					
					
			if pattern.get_cell_source_id(sourceCoordinates) == 0:
				newPattern.set_cell(newCoords, 
				pattern.get_cell_source_id(sourceCoordinates), 
				pattern.get_cell_atlas_coords(sourceCoordinates),
				pattern.get_cell_alternative_tile(sourceCoordinates),
				)
			else:
				newPattern.set_cell(newCoords, 
				pattern.get_cell_source_id(sourceCoordinates), 
				pattern.get_cell_atlas_coords(sourceCoordinates),
				rotation,
				)
					
	return newPattern
	#Ref<TileMapPattern> transformed_pattern;
	#transformed_pattern.instantiate();
	#bool keep_shape = selection_pattern->get_size() == Vector2i(1, 1);
#
	#Vector2i size = selection_pattern->get_size();
	#for (int y = 0; y < size.y; y++) {
		#for (int x = 0; x < size.x; x++) {
			#Vector2i src_coords = Vector2i(x, y);
			#if (!selection_pattern->has_cell(src_coords)) {
				#continue;
			#}
#
			#Vector2i dst_coords;
#
			#if (keep_shape) {
				#dst_coords = src_coords;
			#} else if (p_type == TRANSFORM_ROTATE_LEFT) {
				#dst_coords = Vector2i(y, size.x - x - 1);
			#} else if (p_type == TRANSFORM_ROTATE_RIGHT) {
				#dst_coords = Vector2i(size.y - y - 1, x);
			#} else if (p_type == TRANSFORM_FLIP_H) {
				#dst_coords = Vector2i(size.x - x - 1, y);
			#} else if (p_type == TRANSFORM_FLIP_V) {
				#dst_coords = Vector2i(x, size.y - y - 1);
			#}
#
			#transformed_pattern->set_cell(dst_coords,
					#selection_pattern->get_cell_source_id(src_coords), selection_pattern->get_cell_atlas_coords(src_coords),
					#_get_transformed_alternative(selection_pattern->get_cell_alternative_tile(src_coords), p_type));
		#}
	#}
	#selection_pattern = transformed_pattern;
	#CanvasItemEditor::get_singleton()->update_viewport();
	
	
#func ChooseRoom(coords: Vector2i) -> Room:
	#var returnRoom: Room
	#var patternsCount = tileMap.tile_set.get_patterns_count()
	#for i in range(10):
		#var randRoom = randi_range(0, patternsCount - 1)
		#returnRoom.Index = randRoom
		#if CheckRoom(coords, GetRoomShift(returnRoom)):
			#
			#return returnRoom
	#
	#return null
	#
	#
#func GetRoomShift(room: Room) -> Room:
	#var pattern = tileMap.tile_set.get_pattern(room.Index)
	#
	#for i in pattern.get_used_cells():
		#if pattern.get_cell_source_id(i) == TileType.Entrance:
			#room.Offset = i
	#
	#return room
#
##endregion
	
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	camera.zoom = Vector2(zoomIn, zoomIn)
	Generate()
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _input(event: InputEvent) -> void:
	if InputMap.event_is_action(event, "Reset") && event.is_action_pressed("Reset"):
		reset()
	if InputMap.event_is_action(event, "Zoom") && event.is_action_pressed("Zoom"):
		if camera.zoom.x == zoomIn:
			camera.zoom = Vector2(zoomOut, zoomOut)
		else:
			camera.zoom = Vector2(zoomIn, zoomIn)
		

func sleep() -> void:
	await get_tree().create_timer(timeToWait).timeout
	
func reset() -> void:
	currentTile = Vector2i(0,0)
	branchList.clear()
	generatingCorridor = false
	roomTiles.clear()
	corridorTiles.clear()
	entranceList.clear()
	tileMap.clear_layer(0)
	currentTile = Vector2i(0,0)
	Generate()
