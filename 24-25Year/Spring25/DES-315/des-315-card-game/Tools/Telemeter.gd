extends Node

## The telemetry file
var file : FileAccess

## The path to the telemetry folder, saved in documents
var telemetryPath := OS.get_system_dir(OS.SYSTEM_DIR_DOCUMENTS) + "/" + "CardGameTelemetry"

## Class for all data related to a played hand
class HandData:
	
	func _init(newCardNum: int, newBotNum: int) -> void:
		cardNum = newCardNum
		botNum = newBotNum
	
	## The number of cards in each hand
	var cardNum : int
	## The number of bots in this hand
	var botNum : int
	
	
	## Array of all player cards played
	var playerCardsPlayed : Array[String] = []
	## Array of all cards bot 1 played
	var bot1CardsPlayed : Array[String] = []
	## Array of all cards bot 2 played
	var bot2CardsPlayed : Array[String] = []
	## Array of all cards bot 3 played
	var bot3CardsPlayed : Array[String] = []
	
	## Players end score
	var playerEndScore : int = 0
	## Bot 1's end score
	var bot1EndScore : int = 0
	## Bot 2's end score
	var bot2EndScore : int = 0
	## Bot 3's end score
	var bot3EndScore : int = 0

## Array of all [HandData] instances for each hand played
var data : Array[HandData] = []

## The number of the current hand
var handNum : int = 1

func _ready() -> void:
	# Make sure directory exists
	DirAccess.make_dir_absolute(telemetryPath)
	
	# Open the directory
	var directory = DirAccess.open(telemetryPath)
	
	if not directory.dir_exists(telemetryPath):
		directory.make_dir_recursive(telemetryPath)
		
	var timeDateDict = Time.get_datetime_dict_from_system()
	
	# The file path for this telemetry file
	var filePath = telemetryPath + "/" + "TelemetryData" + "_" + str(timeDateDict.hour) + "_" + str(timeDateDict.minute) + "_" + str(timeDateDict.second) + ".csv"
	
	
	file = FileAccess.open(filePath, FileAccess.WRITE)
	
	assert(file.is_open(), "Error opening telemetry file")


## Called when a new hand is started
func NewHand(cardNum : int, botNum : int) -> void:
	
	# Log the last hand played
	if !data.is_empty():
		ExportHand(data.back())
	
	
	data.push_back(HandData.new(cardNum, botNum))
	
	

## Logs each time cards are played
func CardsPlayed(cards: Array[CardBase]) -> void:
	
	
	var curHand : HandData = data.back()
	
	for i in cards.size():
		if i == 0:
			curHand.playerCardsPlayed.push_back(cards[i].GetCardName())
		if i == 1:
			curHand.bot1CardsPlayed.push_back(cards[i].GetCardName())
		if i == 2:
			curHand.bot2CardsPlayed.push_back(cards[i].GetCardName())
		if i == 3:
			curHand.bot3CardsPlayed.push_back(cards[i].GetCardName())

## Logs each player's score
func EndScores(scores : Array[int]) -> void:
	var curHand : HandData = data.back()
	
	for i in scores.size():
		if i == 0:
			curHand.playerEndScore = scores[i]
		if i == 1:
			curHand.bot1EndScore = scores[i]
		if i == 2:
			curHand.bot2EndScore = scores[i]
		if i == 3:
			curHand.bot3EndScore = scores[i]


## Export a hand to the csv file
func ExportHand(hand : HandData) -> void:
	
	# Each hand is formatted in a specific way using the lines array
	
	var lines : Array[PackedStringArray] = []
	
	var line1 : PackedStringArray
	
	line1.push_back("Hand Num")
	line1.push_back("Num Bots")
	line1.push_back("Num Cards")
	
	lines.push_back(line1)
	
	var line2 : PackedStringArray
	
	line2.push_back(str(handNum))
	line2.push_back(str(hand.botNum))
	line2.push_back(str(hand.cardNum))
	
	lines.push_back(line2)
	
	var line3 : PackedStringArray
	
	line3.push_back("Player Cards")
	line3.push_back("Bot1 Cards")
	line3.push_back("Bot2 Cards")
	line3.push_back("Bot3 Cards")
	
	lines.push_back(line3)
	
	for i in hand.cardNum:
		var newLine : PackedStringArray
		
		newLine.push_back(hand.playerCardsPlayed[i])
		
		newLine.push_back(hand.bot1CardsPlayed[i])
		
		if hand.bot2CardsPlayed.size() > 0:
			newLine.push_back(hand.bot2CardsPlayed[i])
			
		if hand.bot3CardsPlayed.size() > 0:
			newLine.push_back(hand.bot3CardsPlayed[i])
			
		lines.push_back(newLine)
		
		
	var line4 : PackedStringArray
		
	line4.push_back("Player Score")
	line4.push_back("Bot1 Score")
	line4.push_back("Bot2 Score")
	line4.push_back("Bot3 Score")
		
	lines.push_back(line4)
		
	var line5 : PackedStringArray
		
	line5.push_back(str(hand.playerEndScore))
	line5.push_back(str(hand.bot1EndScore))
		
	if hand.botNum > 1:
		line5.push_back(str(hand.bot2EndScore))
			
	if hand.botNum > 2:
		line5.push_back(str(hand.bot3EndScore))
			
	lines.push_back(line5)
		
		
	for line in lines:
		file.store_csv_line(line)
			
	handNum += 1

## Close the file when the game is exited
func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		file.close()
