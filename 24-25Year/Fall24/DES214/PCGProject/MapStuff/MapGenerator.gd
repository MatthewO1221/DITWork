class_name MapGenerator
extends Node2D


#Amount of density for rooms or corridors
enum DensityModes {Sparse, Regular, Cluttered}
#The different corridor types that can be generated
enum CorridorTypes {DrunkWalk, Snaking, Spiral}
#Used for tile or room rotation
enum TileTransform 
{
	ROTATE_0 = 0,
	ROTATE_90 = TileSetAtlasSource.TRANSFORM_TRANSPOSE | TileSetAtlasSource.TRANSFORM_FLIP_H,
	ROTATE_180 = TileSetAtlasSource.TRANSFORM_FLIP_V | TileSetAtlasSource.TRANSFORM_FLIP_H,
	ROTATE_270 = TileSetAtlasSource.TRANSFORM_FLIP_V | TileSetAtlasSource.TRANSFORM_TRANSPOSE,
}
#The cardinal directions
enum Directions 
{
	N = TileSet.CELL_NEIGHBOR_TOP_SIDE,
	E = TileSet.CELL_NEIGHBOR_RIGHT_SIDE,
	S = TileSet.CELL_NEIGHBOR_BOTTOM_SIDE,
	W = TileSet.CELL_NEIGHBOR_LEFT_SIDE
}

#Mode handles all the generation values for a region
class Mode:
	#Name of the mode
	var name: String
	#List of rooms this mode uses
	var roomList: Dictionary
	#How dense the rooms should be
	var roomAmount: DensityModes
	#How dense the corridors should be
	var corridorAmount: DensityModes
	#The enemies that this mode should spawn
	var enemyList: Array
	
	#Minimum corridor length for this mode
	var minCorridorLength: int
	#Maximum corridor length for this mode
	var maxCorridorLength: int
	#The chance for any corridor tile to be a branch
	var branchChance := 0.0
	#The corridor types this mode uses
	var corridorTypes: Array
	#Whether to widen the corridors
	var widen = false
	#The chance that an enemy spawns on a corridor tile
	var corridorEnemyChance := 0.03
	#The chance that an enemy spawns on a room tile
	var roomEnemyChance := 0.03

#A region is an area on the layer where we're generating, acts essentially as a biome
class Region:
	#The name of the region
	var name: String
	#Total tiles in the region
	var totalTiles: int
	#The area this region covers
	var area: Rect2i
	#The generation mode attached to this region
	var mode: Mode

#Handles values for when a room is being placed
class Room:
	#The index of the room we're placing, which pattern it is
	var Index: int
	#Whether the room needs to be offset
	var Offset: Vector2i
	#The current rotation 
	var Rotation: TileTransform

#Maps directions to indices
var directionMap = {
	0: Directions.N,
	1: Directions.E,
	2: Directions.S,
	3: Directions.W
}



@export_group("Debug Values")
#Tile map layer we're drawing things on
@export var tileMapLayer: TileMapLayer
#Time in between placement so you can see generation
@export var timeToWait: float
#The level camera, not the player one
@export var camera: Camera2D
@export_group("")

@export_group("Enemies")
@export var basicEnemy: PackedScene
@export var basicBurstEnemy: PackedScene
@export var bounceEnemy: PackedScene
@export var machineGunEnemy: PackedScene
@export var shotgunEnemy: PackedScene
@export var sniperEnemy: PackedScene
#Important Globals
#The tile we're currently generating from
var currentTile = Vector2i(0,0)
#List of all branch tiles
var branchList: Array
#List of all room tiles
var roomTiles: Array
#List of all corridor tiles
var corridorTiles: Array
#List of all wall tiles
var wallTiles: Array
#List of all normal entrances
var entranceList: Array
#List of entrance tiles to connect all regions together
var entexitList: Array
#List of special entrance tiles
var specialEntrances: Array
#Whether we're panning the camera
var isPanning: bool
#Mouse position last frame
var lastMousePosition
#All the regions
var regions: Array
#All the enemies
var enemies: Array
#Index of empty tiles
const _emptyTile := -1

func Generate() -> void:
	
	var modes = CreateModes()
	
	
	
	for region in regions:
		region.mode = modes.pop_front()
		
		#Place rooms in the level
		SpawnRooms(region)
	
		#Replace all entrance tiles with floors and place them in entrance list
		entranceList.append_array(SweepEntrances())
	
	
		OrganizeEntrances()
	
		ConnectEntrances(region)
	
		CreateBranches(region)
		
		
		
		
		
		
		if region.mode.widen:
			WidenAllCorridors(region)
			
		SpawnEnemies(region)
	
	
	entexitList.append_array(specialEntrances)
	
	OrganizeSpecialEntrances()
	
	var wholeRegion = Region.new()
	
	wholeRegion.mode = Mode.new()
	
	wholeRegion.area = tileMapLayer.get_used_rect()
	
	ConnectSpecialEntrances(wholeRegion)
	
	
		
	PlaceWalls()
	
	
func CreateModes() -> Array:
	var modeArray: Array
	
	var dev1 = Mode.new()
	
	dev1.name = "Development1"
	dev1.roomList = {lShaped = 1, smallRoom = 2, alcoves = 3, largeRoom = 4, columns = 5}
	dev1.roomAmount = DensityModes.Regular
	dev1.corridorAmount = DensityModes.Cluttered
	dev1.minCorridorLength = 3
	dev1.maxCorridorLength = 10
	dev1.branchChance = 0.2
	dev1.corridorTypes.append(CorridorTypes.Snaking)
	dev1.enemyList.append(basicEnemy)
	dev1.enemyList.append(basicBurstEnemy)
	
	
	modeArray.push_back(dev1)
	
	var dev2 = Mode.new()
	
	dev2.name = "Development2"
	dev2.roomList = {blocks = 10, corners = 11, long = 12, cross = 13, diamond = 14}
	dev2.roomAmount = DensityModes.Sparse
	dev2.corridorAmount = DensityModes.Cluttered
	dev2.minCorridorLength = 8
	dev2.maxCorridorLength = 16
	dev2.branchChance = 0.1
	dev2.corridorTypes.append(CorridorTypes.Spiral)
	dev2.enemyList.append(shotgunEnemy)
	dev2.enemyList.append(sniperEnemy)
	
	
	modeArray.push_back(dev2)
	
	var dev3 = Mode.new()
	
	dev3.name = "Development3"
	dev3.roomList = {cave1 = 0, cave2 = 6, cave3 = 7, cave4 = 8, cave5 = 9}
	dev3.roomAmount = DensityModes.Regular
	dev3.corridorAmount = DensityModes.Cluttered
	dev3.minCorridorLength = 15
	dev3.maxCorridorLength = 20
	dev3.branchChance = 0.2
	dev3.corridorTypes.append(CorridorTypes.DrunkWalk)
	dev3.widen = true
	dev3.enemyList.append(machineGunEnemy)
	dev3.enemyList.append(bounceEnemy)
	
	
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

func NewRegion(regionName: String, bottomLeft: Vector2i, topRight: Vector2i) -> Region:
	var newRegion = Region.new()
	
	newRegion.name = regionName
	
	var newRect = Rect2i(bottomLeft.x, topRight.y, topRight.x - bottomLeft.x, bottomLeft.y - topRight.y)
	
	newRegion.area = newRect
	
	newRegion.totalTiles = newRect.get_area()
	
	return newRegion

func ConnectEntrances(region: Region) -> void:
	
	var aStar = AStarGrid2D.new()
	
	
	
	
	aStar.region = region.area
	aStar.cell_size = tileMapLayer.tile_set.tile_size
	
	aStar.default_compute_heuristic = AStarGrid2D.HEURISTIC_MANHATTAN
	aStar.default_estimate_heuristic = AStarGrid2D.HEURISTIC_MANHATTAN
	aStar.diagonal_mode = AStarGrid2D.DIAGONAL_MODE_NEVER
	
	aStar.update()
	
	for tile in roomTiles:
		if aStar.is_in_boundsv(tile):
			aStar.set_point_solid(tile, true)
		
		
	for entrance in entranceList:
		if aStar.is_in_boundsv(entrance):
			aStar.set_point_solid(entrance, false)
		
		
	
	
	for i in range(entranceList.size()):
		if i == entranceList.size() - 1:
			break
		
		ConnectTwoEntrances(region, entranceList[i], entranceList[i + 1] ,aStar)
			
			
func ConnectSpecialEntrances(region: Region) -> void:
	
	
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
		
	
	
	
		
	ConnectTwoEntrances(region, entexitList[0], entexitList[1] ,aStarWhole)
	ConnectTwoEntrances(region, entexitList[2], entexitList[3] ,aStarWhole)
	ConnectTwoEntrances(region, entexitList[3], entexitList[4] ,aStarWhole)
	ConnectTwoEntrances(region, entexitList[5], entexitList[6] ,aStarWhole)
	ConnectTwoEntrances(region, entexitList[6], entexitList[7] ,aStarWhole)
	ConnectTwoEntrances(region, entexitList[8], entexitList[9] ,aStarWhole)
		
		
		
		
func ConnectTwoEntrances(region: Region, tile: Vector2i, nextEntrance: Vector2i, grid: AStarGrid2D) -> void:
		
		
		
	if nextEntrance == null:
		return
		
	var path = grid.get_id_path(tile, nextEntrance, true)
		
	for tiles in path:
		PlaceTile(region, tiles)
			
		
func CreateBranches(region: Region) -> void:
	
	#Make regions connect
	
	
	while !branchList.is_empty():
		var branch = branchList.pop_front()
		SpawnBranch(branch, region)
		
		match region.mode.corridorAmount:
			DensityModes.Sparse when float(corridorTiles.size())/region.totalTiles >= 0.25:
				break
			DensityModes.Regular when float(corridorTiles.size())/region.totalTiles >= 0.5:
				break
			DensityModes.Cluttered when float(corridorTiles.size())/region.totalTiles >= 0.9:
				break
		

func SpawnBranch(branch: Vector2i, region: Region) -> void:
	
	
	var branchType = region.mode.corridorTypes.pick_random()
	
	
	match branchType:
		CorridorTypes.DrunkWalk:
			SpawnDrunkCorridor(branch, region)
		CorridorTypes.Snaking:
			SpawnSnakingCorridor(branch, region)
		CorridorTypes.Spiral:
			SpawnSpiralCorridor(branch, region)
	

func SpawnDrunkCorridor(start: Vector2i, region: Region) -> void:
	var mode = region.mode
	var length = randi_range(mode.minCorridorLength, mode.maxCorridorLength)
	var tile = start
	
	for i in range(length):
	
		for j in range(10):
			var dir = randi_range(0,3)
		
			if IsValidNeighbor(tile, directionMap[dir], region):
				tile = PlaceTileInDirection(region, tile, directionMap[dir])
				break
	
func SpawnSnakingCorridor(start: Vector2i, region: Region) -> void:
	var mode = region.mode
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
				tile = PlaceTileInDirection(region, tile, dir)
		horizontal = !horizontal
	
	
func SpawnSpiralCorridor(start: Vector2i, region: Region) -> void:
	var mode = region.mode
	var tile = start
	
	var dir = randi_range(0,3)
	var length = randi_range(mode.minCorridorLength, mode.maxCorridorLength)
	
	for i in range(10):
		for j in range(length):
			if IsValidNeighbor(tile, directionMap[dir], region):
				tile = PlaceTileInDirection(region, tile, directionMap[dir])
				
				
		match dir:
			0:
				dir = 1
			1:
				dir = 2
			2:
				dir = 3
			3:
				dir = 0
		
	

func WidenAllCorridors(region: Region) -> void:
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
	
	for tile in allFloors:
		for direction in directions:
			var neighbor = tileMapLayer.get_neighbor_cell(tile, direction)
			
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

func SpawnRooms(region: Region) -> void:
	var mode = region.mode
	var roomDensity = mode.roomAmount
	var totalTiles = region.totalTiles
	var tilesPlaced = 0
	
	while true:
		
		var placeTile = GetRandomTile(region)
		
		for i in range(10):
			var roomToTry = GetRandomRoom(mode)
			var randRot = TileTransform.values().pick_random()
			var canBePlaced = CheckRoom(placeTile, roomToTry, randRot, region)
			
			if canBePlaced:
				PlaceRoom(roomToTry, placeTile, randRot)
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
	
	
	
	
	var patternIndex = mode.roomList.values().pick_random()
	
	var pattern = tileMapLayer.tile_set.get_pattern(patternIndex)
	
	return pattern
	
	
func PlaceRoom(room: TileMapPattern, coords: Vector2i, rot: TileTransform) -> void:
	var placedRoom = RotateRoom(room, rot)
	
	var tiles = placedRoom.get_used_cells()
	
	for tile in tiles:
		roomTiles.append(tileMapLayer.map_pattern(coords, tile, placedRoom))
	
	
	
	tileMapLayer.set_pattern(coords, placedRoom)
	
	
	

func PlaceTile(region: Region, coords: Vector2i) -> Vector2i:
	var mode = region.mode
	tileMapLayer.set_cell(coords, 3, Vector2i(1,1))
	corridorTiles.append(coords)
	var rando = randf_range(0, 1.0)
	if rando < mode.branchChance:
		branchList.push_front(coords)
	
	
	return coords
	
	
func PlaceTileInDirection(region: Region, coords:Vector2i, dir: Directions) -> Vector2i:
	return PlaceTile(region, GetCoordsInDir(coords, dir))
	
	

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
	

	
	
	
func CheckRoom(coords: Vector2i, room: TileMapPattern, rot: TileTransform, region: Region) -> bool:
	
	var rotatedRoom = RotateRoom(room, rot)
	var roomUsed = rotatedRoom.get_used_cells()
	
	
	var smallerRegion = region.area.grow(-1)
	
	
	
	for i in roomUsed:
		var curTile = tileMapLayer.map_pattern(coords, i, rotatedRoom)
		if tileMapLayer.get_cell_source_id(curTile) != _emptyTile or !smallerRegion.has_point(curTile):
			return false
		
	return true


func SpawnEnemies(region: Region) -> void:
	
	for roomTile in roomTiles:
		if region.area.has_point(roomTile) and tileMapLayer.get_cell_atlas_coords(roomTile) != Vector2i(5,0):
			var chance = randf_range(0.0, 1.0)
			
			if chance <= region.mode.roomEnemyChance:
				SpawnRandomEnemy(region, roomTile)
				
	for corridorTile in corridorTiles:
		if region.area.has_point(corridorTile):
			var chance = randf_range(0.0, 1.0)
			
			if chance <= region.mode.corridorEnemyChance:
				SpawnRandomEnemy(region, corridorTile)
	
func SpawnRandomEnemy(region: Region, coords: Vector2i) -> void:
	var enemyToSpawn = GetRandomEnemy(region)
	
	var enemyInstance = enemyToSpawn.instantiate()
	
	var globalCoords = tileMapLayer.to_global(tileMapLayer.map_to_local(coords))
	
	get_tree().current_scene.add_child(enemyInstance)
	
	enemyInstance.global_position = globalCoords
	
	enemies.append(enemyInstance)
	
	
func GetRandomEnemy(region: Region) -> PackedScene:
	return region.mode.enemyList.pick_random()
	
func RotateRoom(pattern: TileMapPattern, rot: TileTransform) -> TileMapPattern:
	
	if rot == TileTransform.ROTATE_0:
		return pattern
	
	var newPattern := TileMapPattern.new()
	
	
	var size = pattern.get_size()
	
	for y in range(size.y):
		for x in range(size.x):
			var sourceCoordinates = Vector2i(x, y)
			
			if !pattern.has_cell(sourceCoordinates):
				continue
			
			var newCoords: Vector2i
			
			
			match rot:
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
				rot,
				)
					
	return newPattern

	
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	
	regions = CreateRegions()
	
	var specialEntrancesL = SweepSpecialEntrances()
	specialEntrances.append_array(specialEntrancesL)
	Generate()
	




func _input(event: InputEvent) -> void:
	if InputMap.event_is_action(event, "Reset") && event.is_action_pressed("Reset"):
		reset()
	
			
	if InputMap.event_is_action(event, "Zoom In") and event.is_action_pressed("Zoom In"):
		var zoomAmount = camera.zoom.x
		camera.zoom = Vector2(zoomAmount + 0.1, zoomAmount + 0.1)
		
	if InputMap.event_is_action(event, "Zoom Out") and event.is_action_pressed("Zoom Out"):
		var zoomAmount = camera.zoom.x
		camera.zoom = Vector2(zoomAmount - 0.1, zoomAmount - 0.1)
	
	
	
	
	
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
	
	for enemy in enemies:
		enemy.queue_free()
	
	enemies.clear()
	
	currentTile = Vector2i(0,0)
	Generate()
