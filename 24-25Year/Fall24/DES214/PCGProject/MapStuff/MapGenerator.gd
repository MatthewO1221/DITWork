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
@export var tileMapLayer: TileMapLayer
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

enum CorridorTypes
{
	DrunkWalk,
	Snaking,
	Spiral
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
	var branchChance := 0.0
	var corridorTypes: Array
	var widen = false


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

var wallTiles: Array

var entranceList: Array

var entexitList: Array

var specialEntrances: Array

var isPanning: bool
var lastMousePosition

var regions: Array

func Generate() -> void:
	
	var modes = CreateModes()
	
	
	
	for region in regions:
		var curMode = modes.pop_front()
		
		#Place rooms in the level
		SpawnRooms(curMode, region)
	
		#Replace all entrance tiles with floors and place them in entrance list
		entranceList.append_array(SweepEntrances())
	
	
		OrganizeEntrances()
	
		ConnectEntrances(curMode, region)
	
		CreateBranches(curMode, region)
		
		
		
		
		
		
		if curMode.widen:
			WidenAllCorridors(curMode, region)
			
	
	
	entexitList.append_array(specialEntrances)
	
	OrganizeSpecialEntrances()
	
	var wholeRegion = Region.new()
	
	wholeRegion.area = tileMapLayer.get_used_rect()
	
	ConnectSpecialEntrances(Mode.new(), wholeRegion)
	
	
		
	PlaceWalls()
	
func CreateModes() -> Array:
	var modeArray: Array
	
	var dev1 = Mode.new()
	
	dev1.name = "Development1"
	dev1.roomList = {lShaped = 0, smallRoom = 1, alcoves = 2, largeRoom = 3, columns = 4}
	dev1.roomAmount = DensityModes.Regular
	dev1.corridorAmount = DensityModes.Cluttered
	dev1.minCorridorLength = 3
	dev1.maxCorridorLength = 10
	dev1.branchChance = 0.2
	dev1.corridorTypes.append(CorridorTypes.Snaking)
	
	
	modeArray.push_back(dev1)
	
	var dev2 = Mode.new()
	
	dev2.name = "Development2"
	dev2.roomList = {lShaped = 0, smallRoom = 1, alcoves = 2, largeRoom = 3, columns = 4}
	dev2.roomAmount = DensityModes.Sparse
	dev2.corridorAmount = DensityModes.Cluttered
	dev2.minCorridorLength = 8
	dev2.maxCorridorLength = 16
	dev2.branchChance = 0.1
	dev2.corridorTypes.append(CorridorTypes.Spiral)
	
	modeArray.push_back(dev2)
	
	var dev3 = Mode.new()
	
	dev3.name = "Development3"
	dev3.roomList = {lShaped = 0, smallRoom = 1, alcoves = 2, largeRoom = 3, columns = 4}
	dev3.roomAmount = DensityModes.Regular
	dev3.corridorAmount = DensityModes.Cluttered
	dev3.minCorridorLength = 15
	dev3.maxCorridorLength = 20
	dev3.branchChance = 0.2
	dev3.corridorTypes.append(CorridorTypes.DrunkWalk)
	dev3.widen = true
	
	
	modeArray.push_back(dev3)
	#var name: String
	#var roomList: Dictionary
	#var roomAmount: DensityModes
	#var corridorList: Dictionary
	#var corridorAmount: DensityModes
	#var enemyList: Dictionary
	#var lootList: Dictionary
	#
	#var minCorridorLength: int
	#var maxCorridorLength: int
	#var branchChance: float
	#var corridorTypes: Array
	
	return modeArray

func CreateRegions() -> Array:
	var regionTiles = tileMapLayer.get_used_cells_by_id(0, Vector2i(-1,-1), 5)
	
	var regionArray: Array
	
	regionTiles.sort_custom(OrganizeTiles)
	var i = 1
	
	while !regionTiles.is_empty():
		var lowerLeft = regionTiles.pop_front()
		var topRight = regionTiles.pop_front()
		
		regionArray.push_back(NewRegion("Development" + str(i), lowerLeft, topRight))
		i += 1
		tileMapLayer.erase_cell(lowerLeft)
		tileMapLayer.erase_cell(topRight)
		
	return regionArray

func NewRegion(name: String, bottomLeft: Vector2i, topRight: Vector2i) -> Region:
	var newRegion = Region.new()
	
	newRegion.name = name
	
	var newRect = Rect2i(bottomLeft.x, topRight.y, topRight.x - bottomLeft.x, bottomLeft.y - topRight.y)
	
	newRegion.area = newRect
	
	newRegion.totalTiles = newRect.get_area()
	
	return newRegion

func ConnectEntrances(mode: Mode, region: Region) -> void:
	
	var aStar = AStarGrid2D.new()
	
	
	
	
	aStar.region = region.area
	aStar.cell_size = tileMapLayer.tile_set.tile_size
	
	aStar.default_compute_heuristic = AStarGrid2D.HEURISTIC_MANHATTAN
	aStar.default_estimate_heuristic = AStarGrid2D.HEURISTIC_MANHATTAN
	aStar.diagonal_mode = AStarGrid2D.DIAGONAL_MODE_NEVER
	
	aStar.update()
	
	for tile in roomTiles:
		aStar.set_point_solid(tile, true)
		
		
	for entrance in entranceList:
		aStar.set_point_solid(entrance, false)
		
		
	
	
	for i in range(entranceList.size()):
		if i == entranceList.size() - 1:
			break
		
		ConnectTwoEntrances(mode, entranceList[i], entranceList[i + 1] ,aStar)
			
			
func ConnectSpecialEntrances(mode: Mode, region: Region) -> void:
	
	
	var aStarWhole = AStarGrid2D.new()
	
	aStarWhole.region = tileMapLayer.get_used_rect()
	aStarWhole.cell_size = tileMapLayer.tile_set.tile_size
	
	aStarWhole.default_compute_heuristic = AStarGrid2D.HEURISTIC_MANHATTAN
	aStarWhole.default_estimate_heuristic = AStarGrid2D.HEURISTIC_MANHATTAN
	aStarWhole.diagonal_mode = AStarGrid2D.DIAGONAL_MODE_NEVER
	
	aStarWhole.update()
	
	
	for tile in roomTiles:
		aStarWhole.set_point_solid(tile, true)
		
	for entrance in entexitList:
		aStarWhole.set_point_solid(entrance, false)
		
	
	
	
		
	ConnectTwoEntrances(mode, entexitList[0], entexitList[1] ,aStarWhole)
	ConnectTwoEntrances(mode, entexitList[2], entexitList[3] ,aStarWhole)
	ConnectTwoEntrances(mode, entexitList[3], entexitList[4] ,aStarWhole)
	ConnectTwoEntrances(mode, entexitList[5], entexitList[6] ,aStarWhole)
	ConnectTwoEntrances(mode, entexitList[6], entexitList[7] ,aStarWhole)
	ConnectTwoEntrances(mode, entexitList[8], entexitList[9] ,aStarWhole)
		
		
		
		
func ConnectTwoEntrances(mode: Mode, tile: Vector2i, nextEntrance: Vector2i, grid: AStarGrid2D) -> void:
		
		
		
	if nextEntrance == null:
		return
		
	var path = grid.get_id_path(tile, nextEntrance, true)
		
	for tiles in path:
		PlaceTile(mode, tiles)
			
		
func CreateBranches(mode: Mode, region: Region) -> void:
	
	#Make regions connect
	
	
	while !branchList.is_empty():
		var branch = branchList.pop_front()
		SpawnBranch(branch, mode, region)
		
		match mode.corridorAmount:
			DensityModes.Sparse when float(corridorTiles.size())/region.totalTiles >= 0.25:
				break
			DensityModes.Regular when float(corridorTiles.size())/region.totalTiles >= 0.5:
				break
			DensityModes.Cluttered when float(corridorTiles.size())/region.totalTiles >= 0.9:
				break
		

func SpawnBranch(branch: Vector2i, mode: Mode, region: Region) -> void:
	var currentTile = branch
	
	var branchType = mode.corridorTypes.pick_random()
	
	
	match branchType:
		CorridorTypes.DrunkWalk:
			SpawnDrunkCorridor(branch, mode, region)
		CorridorTypes.Snaking:
			SpawnSnakingCorridor(branch, mode, region)
		CorridorTypes.Spiral:
			SpawnSpiralCorridor(branch, mode, region)
	

func SpawnDrunkCorridor(start: Vector2i, mode: Mode, region: Region) -> void:
	var length = randi_range(mode.minCorridorLength, mode.maxCorridorLength)
	var tile = start
	
	for i in range(length):
	
		for j in range(10):
			var dir = randi_range(0,3)
		
			if IsValidNeighbor(tile, directionMap[dir], region):
				tile = await PlaceTileInDirection(mode, tile, directionMap[dir])
				break
	
func SpawnSnakingCorridor(start: Vector2i, mode: Mode, region: Region) -> void:
	var horizontal = true
	var dir
	var turns = randi_range(2,7)
	var tile = start
	for i in range(turns):
		var length = randi_range(mode.minCorridorLength, mode.maxCorridorLength)
		if horizontal:
			dir = [TileSet.CellNeighbor.CELL_NEIGHBOR_RIGHT_SIDE, TileSet.CellNeighbor.CELL_NEIGHBOR_LEFT_SIDE].pick_random()
		else:
			dir = [TileSet.CellNeighbor.CELL_NEIGHBOR_TOP_SIDE, TileSet.CellNeighbor.CELL_NEIGHBOR_BOTTOM_SIDE].pick_random()
			
		for j in range(length):
			if IsValidNeighbor(tile, dir, region):
				tile = await PlaceTileInDirection(mode, tile, dir)
		horizontal = !horizontal
	
	
func SpawnSpiralCorridor(start: Vector2i, mode: Mode, region: Region) -> void:
	var tile = start
	
	var dir = randi_range(0,3)
	var length = randi_range(mode.minCorridorLength, mode.maxCorridorLength)
	
	for i in range(10):
		for j in range(length):
			if IsValidNeighbor(tile, directionMap[dir], region):
				tile = PlaceTileInDirection(mode, tile, directionMap[dir])
				
				
		match dir:
			0:
				dir = 1
			1:
				dir = 2
			2:
				dir = 3
			3:
				dir = 0
		
	

func WidenAllCorridors(mode: Mode, region: Region) -> void:
	var allWalls = tileMapLayer.get_used_cells_by_id(3, Vector2i(5,0))
	
	var directions = [TileSet.CELL_NEIGHBOR_TOP_SIDE, 
	TileSet.CELL_NEIGHBOR_RIGHT_SIDE,
	TileSet.CELL_NEIGHBOR_BOTTOM_SIDE,
	TileSet.CELL_NEIGHBOR_LEFT_SIDE]
	
	for wall in allWalls:
		var numNeighbors = 0
		for direction in directions:
			if region.area.has_point(wall):
				var neighbor = tileMapLayer.get_neighbor_cell(wall, direction)
			
				if tileMapLayer.get_cell_source_id(neighbor) == 3 and tileMapLayer.get_cell_atlas_coords(neighbor) == Vector2i(1,1):
					numNeighbors += 1
		if numNeighbors >= 3:
			tileMapLayer.set_cell(wall, 3, Vector2i(1,1))
			corridorTiles.append(wall)
	
func PlaceWalls() -> void:
	var allFloors = tileMapLayer.get_used_cells_by_id(3, Vector2i(1,1))
	
	var directions = [TileSet.CELL_NEIGHBOR_TOP_SIDE, 
	TileSet.CELL_NEIGHBOR_TOP_RIGHT_CORNER,
	TileSet.CELL_NEIGHBOR_RIGHT_SIDE,
	TileSet.CELL_NEIGHBOR_BOTTOM_RIGHT_CORNER,
	TileSet.CELL_NEIGHBOR_BOTTOM_SIDE,
	TileSet.CELL_NEIGHBOR_BOTTOM_LEFT_CORNER,
	TileSet.CELL_NEIGHBOR_LEFT_SIDE,
	TileSet.CELL_NEIGHBOR_TOP_LEFT_CORNER]
	
	for floor in allFloors:
		for direction in directions:
			var neighbor = tileMapLayer.get_neighbor_cell(floor, direction)
			
			if tileMapLayer.get_cell_source_id(neighbor) == -1:
				tileMapLayer.set_cell(neighbor, 3, Vector2i(5,0))
				wallTiles.push_back(neighbor)
		
	
func SweepEntrances() -> Array[Vector2i]:
	var allTiles = tileMapLayer.get_used_cells_by_id(0, Vector2i(-1,-1), 3)
	
	for tile in allTiles:
		tileMapLayer.set_cell(tile, 3, Vector2i(1,1))
		
	allTiles.sort_custom(OrganizeTiles)
	
	entexitList.push_back(allTiles.front())
	entexitList.push_back(allTiles.back())
	
	return allTiles
	
func SweepSpecialEntrances() -> Array[Vector2i]:
	var allTiles = tileMapLayer.get_used_cells_by_id(0, Vector2i(), 6)
	
	for tile in allTiles:
		tileMapLayer.set_cell(tile, 3, Vector2i(1,1))
		
	return allTiles
		
		
func OrganizeEntrances() -> void:
	var newList = entranceList
	
	newList.sort_custom(OrganizeTiles)
		
	entranceList = newList
	
	
		

func OrganizeSpecialEntrances() -> void:
	var newList = entexitList
	
	newList.sort_custom(OrganizeTiles)
	
	entexitList = newList
	

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
			var canBePlaced = CheckRoom(placeTile, roomToTry, rotation, region)
			
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
	
	var rect = region.area
	
	returnTile.x = randi_range(rect.position.x, rect.position.x + rect.size.x - 1)
	returnTile.y = randi_range(rect.position.y, rect.position.y + rect.size.y - 1)
	
	return returnTile

func GetRandomRoom(mode: Mode) -> TileMapPattern:
	var patternsCount = mode.roomList.size()
	
	var rand = randi_range(0, patternsCount)
	
	var patternIndex = mode.roomList.values().pick_random()
	
	var pattern = tileMapLayer.tile_set.get_pattern(patternIndex)
	
	return pattern
	
	
func PlaceRoom(room: TileMapPattern, coords: Vector2i, rotation: TileTransform) -> void:
	var placedRoom = RotateRoom(room, rotation)
	
	var tiles = placedRoom.get_used_cells()
	
	for tile in tiles:
		roomTiles.append(tileMapLayer.map_pattern(coords, tile, placedRoom))
	
	
	
	tileMapLayer.set_pattern(coords, placedRoom)
	
	
	

func PlaceTile(mode: Mode, coords: Vector2i) -> Vector2i:
	tileMapLayer.set_cell(coords, 3, Vector2i(1,1))
	corridorTiles.append(coords)
	var rando = randf_range(0, 1.0)
	if rando < mode.branchChance:
		branchList.push_front(coords)
	
	
	return coords
	
	
func PlaceTileInDirection(mode: Mode, coords:Vector2i, dir: Directions) -> Vector2i:
	return PlaceTile(mode, GetCoordsInDir(coords, dir))
	
	

func IsValidNeighbor(parent: Vector2i, dir: TileSet.CellNeighbor, region: Region) -> bool:
	var newTile = tileMapLayer.get_neighbor_cell(parent, dir)
	
	if !IsValidPlacement(newTile, region):
		return false
	
	for i in range(4):
		var adjacentTile = tileMapLayer.get_neighbor_cell(newTile, directionMap[i])
		if corridorTiles.has(adjacentTile) and adjacentTile != parent:
			return false
	
	return true

func IsValidPlacement(spot: Vector2i, region: Region) -> bool:
	var id = tileMapLayer.get_cell_source_id(spot)
	var atlasCoords = tileMapLayer.get_cell_atlas_coords(spot)
	
	if !region.area.has_point(spot):
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
	

	
	
	
func CheckRoom(coords: Vector2i, room: TileMapPattern, rotation: TileTransform, region: Region) -> bool:
	
	var rotatedRoom = RotateRoom(room, rotation)
	var roomTiles = rotatedRoom.get_used_cells()
	var foundPlacement = false
	
	var smallerRegion = region.area.grow(-1)
	
	
	
	for i in roomTiles:
		var curTile = tileMapLayer.map_pattern(coords, i, rotatedRoom)
		if tileMapLayer.get_cell_source_id(curTile) != TileType.Empty or !smallerRegion.has_point(curTile):
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

	
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	camera.zoom = Vector2(zoomIn, zoomIn)
	regions = CreateRegions()
	
	var specialEntrancesL = SweepSpecialEntrances()
	specialEntrances.append_array(specialEntrancesL)
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
			
	if InputMap.event_is_action(event, "Zoom In") and event.is_action_pressed("Zoom In"):
		var zoomAmount = camera.zoom.x
		camera.zoom = Vector2(zoomAmount + 0.1, zoomAmount + 0.1)
		
	if InputMap.event_is_action(event, "Zoom Out") and event.is_action_pressed("Zoom Out"):
		var zoomAmount = camera.zoom.x
		camera.zoom = Vector2(zoomAmount - 0.1, zoomAmount - 0.1)
	
	
	
	
	var dragSens = 1.0
	# Start panning when the middle mouse button is pressed
	if InputMap.event_is_action(event, "Panning"):
		if event.is_pressed():
			isPanning = true
			lastMousePosition = get_viewport().get_mouse_position()
		elif event.is_released():
			isPanning = false

	# Handle panning movement
	if isPanning and event is InputEventMouseMotion:
		var mouseDelta = event.position - lastMousePosition
		camera.position -= mouseDelta
		lastMousePosition = event.position

func sleep() -> int:
	await get_tree().create_timer(timeToWait).timeout
	return 1
	
func reset() -> void:
	currentTile = Vector2i(0,0)
	branchList.clear()
	generatingCorridor = false
	
	for tile in corridorTiles:
		tileMapLayer.erase_cell(tile)
		
	for tile in roomTiles:
		tileMapLayer.erase_cell(tile)
		
	for wall in wallTiles:
		tileMapLayer.erase_cell(wall)
		
	roomTiles.clear()
	corridorTiles.clear()
	entranceList.clear()
	entexitList.clear()
	wallTiles.clear()
	
	
	
	
	currentTile = Vector2i(0,0)
	Generate()
