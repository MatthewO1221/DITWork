class_name GameBoard
extends Node2D

var delay := 0.1

## Cards per hand
@export var handSize := 7
## @deprecated
@export var playSpeed := 1.0
## Number of bots
@export var botNum := 3

## The deck
@onready var deck = $DeckContainer

## The player's hand
@onready var playerHand := $Player as HandContainer
## Bot 1's hand
@onready var botHand1 := $Bot1 as HandContainer
## Bot 2's hand
@onready var botHand2 := $Bot2 as HandContainer
## Bot 3's hand
@onready var botHand3 := $Bot3 as HandContainer

## Action list
var actionList := ActionList.new()

## Player's score
var playerScore := 0
## Bot 1's score
var bot1Score := 0
## Bot 2's score
var bot2Score := 0
## Bot 3's score
var bot3Score := 0

# All of this is dumb
@onready var botName1 = $Control/BotLabel1
@onready var botName2 = $Control/BotLabel2
@onready var botName3 = $Control/BotLabel3
@onready var botScore1 = $Control/BotScore1
@onready var botScore2 = $Control/BotScore2
@onready var botScore3 = $Control/BotScore3

## Whether debug text should be displayed
var debugMode : bool = false

## Sent when hand is finished playing
signal DonePlaying

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	# Spawn a deck and shuffle it
	deck.SpawnDeck()
	ShuffleDeck()
	# Makes card hovering work better
	get_viewport().set_physics_object_picking_sort(true)
	get_viewport().set_physics_object_picking_first_only(true)
	playerScore = 0
	bot1Score = 0
	bot2Score = 0
	bot3Score = 0

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	actionList.UpdateAllActions(delta)
	
	if debugMode:
		DisplayDebugText()


func _input(event: InputEvent) -> void:
	#if event.is_action_pressed("Play Cards"):
		#PlayCards()
	if event.is_action_pressed("Debug"):
		debugMode = !debugMode
		
		if debugMode:
			$ActionDebugger.show()
		else:
			$ActionDebugger.hide()

## Deals [GameBoard.handSize] cards to each hand
func DealHands() -> void:
	
	var hands : Array[HandContainer] = [playerHand]

	match botNum:
		1:
			hands.push_back(botHand1)
		2:
			hands.push_back(botHand1)
			hands.push_back(botHand2)
		3:
			hands.push_back(botHand1)
			hands.push_back(botHand2)
			hands.push_back(botHand3)
	
		
	var dealCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
	var dealAction = DealHandsAction.new(false, false, "Deal", 0.5, 0.0, false, hands, handSize, deck, 0.1, dealCurve)
	
	actionList.PushBack(dealAction)

## Shuffles the deck
func ShuffleDeck() -> void:
	
	var shuffleCurve := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN)
	
	var shuffleAction = ShuffleAction.new(true, false, "Shuffle", 1.0, 0.0, false, deck, shuffleCurve)
	
	actionList.PushBack(shuffleAction)
	
## Flips all cards in player hand
func FlipHand() -> void:
	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN)
	
	for card in playerHand.hand.keys():
		var flipAction = FlipCardAction.new(true, false, "HandFlip", 1.0, 0.0, false, card, easingMethod)
		actionList.PushBack(flipAction)

## Plays cards from each hand, given card is picked from player hand, others are random
func PlayCards(card: CardBase) -> void:
	var trickContainer = $TrickContainer as TrickContainer
	
	
	trickContainer.GrabCard(playerHand, card)
	
	var hands : Array[HandContainer] = []

	match botNum:
		1:
			hands.push_back(botHand1)
		2:
			hands.push_back(botHand1)
			hands.push_back(botHand2)
		3:
			hands.push_back(botHand1)
			hands.push_back(botHand2)
			hands.push_back(botHand3)
	
	
	trickContainer.GrabCards(hands)


func PlayHand() -> void:
	if playerHand.hand.keys().size() > 0:
	
		var trickContainer = $TrickContainer as TrickContainer
		
		
		trickContainer.GrabCard(playerHand, playerHand.hand.keys().pick_random())
		
		var hands : Array[HandContainer] = []

		match botNum:
			1:
				hands.push_back(botHand1)
			2:
				hands.push_back(botHand1)
				hands.push_back(botHand2)
			3:
				hands.push_back(botHand1)
				hands.push_back(botHand2)
				hands.push_back(botHand3)
		
		
		trickContainer.GrabCards(hands)
		
		await trickContainer.DoneScoring
		
		PlayHand()
		
	else:
		var trickContainer = $TrickContainer as TrickContainer
		
		await trickContainer.DoneScoring
		
		DonePlaying.emit()

func ScoreTrick() -> void:
	var winningCards = $TrickContainer.GetWinningCard() as Array[CardBase]
	
	

func _on_bot_1_hover_area_mouse_entered() -> void:
	var customCurve = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var nameFadeAction = FadeAction.new(false, false, "Bot1Fade", 1.0, 0.0, false, botName1, 1.0, customCurve)
	
	actionList.PushBack(nameFadeAction)
	
	var scoreFadeAction = FadeAction.new(true, false, "Bot1Fade", 1.0, 0.0, false, botScore1, 1.0, customCurve)
	
	actionList.PushBack(scoreFadeAction)

func _on_bot_2_hover_area_mouse_entered() -> void:
	var customCurve = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var nameFadeAction = FadeAction.new(false, false, "Bot2Fade", 1.0, 0.0, false, botName2, 1.0, customCurve)
	
	actionList.PushBack(nameFadeAction)
	
	var scoreFadeAction = FadeAction.new(true, false, "Bot2Fade", 1.0, 0.0, false, botScore2, 1.0, customCurve)
	
	actionList.PushBack(scoreFadeAction)


func _on_bot_3_hover_area_mouse_entered() -> void:
	var customCurve = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var nameFadeAction = FadeAction.new(false, false, "Bot3Fade", 1.0, 0.0, false, botName3, 1.0, customCurve)
	
	actionList.PushBack(nameFadeAction)
	
	var scoreFadeAction = FadeAction.new(true, false, "Bot3Fade", 1.0, 0.0, false, botScore3, 1.0, customCurve)
	
	actionList.PushBack(scoreFadeAction)


func _on_bot_1_hover_area_mouse_exited() -> void:
	var customCurve = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var nameFadeAction = FadeAction.new(false, false, "Bot1Fade", 1.0, 0.0, false, botName1, 0.0, customCurve)
	
	actionList.PushBack(nameFadeAction)
	
	var scoreFadeAction = FadeAction.new(true, false, "Bot1Fade", 1.0, 0.0, false, botScore1, 0.0, customCurve)
	
	actionList.PushBack(scoreFadeAction)


func _on_bot_2_hover_area_mouse_exited() -> void:
	var customCurve = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var nameFadeAction = FadeAction.new(false, false, "Bot2Fade", 1.0, 0.0, false, botName2, 0.0, customCurve)
	
	actionList.PushBack(nameFadeAction)
	
	var scoreFadeAction = FadeAction.new(true, false, "Bot2Fade", 1.0, 0.0, false, botScore2, 0.0, customCurve)
	
	actionList.PushBack(scoreFadeAction)


func _on_bot_3_hover_area_mouse_exited() -> void:
	var customCurve = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var nameFadeAction = FadeAction.new(false, false, "Bot3Fade", 1.0, 0.0, false, botName3, 0.0, customCurve)
	
	actionList.PushBack(nameFadeAction)
	
	var scoreFadeAction = FadeAction.new(true, false, "Bot3Fade", 1.0, 0.0, false, botScore3, 0.0, customCurve)
	
	actionList.PushBack(scoreFadeAction)


func UpdatePlaySpeed(newValue: float) -> void:
	playSpeed = newValue
	
	Engine.time_scale = playSpeed
	
func UpdateHandSize(newValue : int) -> void:
	handSize = newValue

func UpdateHandNumber(newValue : int) -> void:
	
	var addingHands : bool = botNum < newValue
	
	
	
	
	if addingHands:
		if botNum < 2 and newValue >= 2:
			var dealCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
			var dealAction = DealHandsAction.new(false, false, "Deal", 0.5, 0.0, false, [botHand2], botHand1.hand.size(), deck, 0.1, dealCurve)
	
			actionList.PushBack(dealAction)
			
		if botNum < 3 and newValue >= 3:
			var dealCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
			var dealAction = DealHandsAction.new(false, false, "Deal", 0.5, 0.0, false, [botHand3], botHand1.hand.size(), deck, 0.1, dealCurve)
	
			actionList.PushBack(dealAction)
	else:
		if newValue < 3:
			deck.GrabCards(botHand3.hand.keys())
			for card in botHand3.hand.keys():
				botHand3.RemoveCard(card)
			
		if newValue < 2:
			deck.GrabCards(botHand2.hand.keys())
			
			for card in botHand2.hand.keys():
				botHand2.RemoveCard(card)
				
		ShuffleDeck()
	
	botNum = newValue


func DisplayDebugText() -> void:
	var allContainers = get_tree().get_nodes_in_group("Container")
	
	var fullText : String = ""
	
	fullText = fullText + GetActionListText(actionList)
	
	for container in allContainers:
		fullText = fullText + GetActionListText(container.actionList)
	
	$ActionDebugger/RichTextLabel.text = fullText
	
	
func GetActionListText(list: ActionList) -> String:
	var returnString : String = ""
	
	var actionDict = list.lists
	
	for key in actionDict.keys():
		
		var actions : Array[Action] = actionDict[key]
		
		if actions.is_empty():
			continue
		
		returnString = returnString + key + "\n"
		
		for action in actions:
			var children = action.children
			
				
			
			returnString = returnString + "\t" + action.GetActionType()
			
			if "duration" in action and "timePassed" in action:
				returnString += " | " + str(snappedf(action.TimeLeft(), 0.01))
			
			returnString += "\n"
			for child in children:
				var grandChildren = child.children
				
				returnString = returnString + "\t\t" + child.GetActionType()
				
				if "duration" in child and "timePassed" in child:
					returnString += " | " + str(snappedf(child.TimeLeft(), 0.01))
					
				returnString += "\n"
				
				for grandChild in grandChildren:
					
					returnString = returnString + "\t\t\t" + grandChild.GetActionType()
					
					if "duration" in grandChild and "timePassed" in grandChild:
						returnString += " | " + str(snappedf(grandChild.TimeLeft(), 0.01))
						
					returnString += "\n"
		

	return returnString


func AddScore(winners: Dictionary[HandContainer, CardBase]) -> void:
	
	$Control/WinnerText.win(winners)
	
	for hand in winners.keys():
		var winningCard = winners[hand]
		
		match hand:
			playerHand:
				playerScore += 1
			botHand1:
				bot1Score += 1
			botHand2:
				bot2Score += 1
			botHand3:
				bot3Score += 1
	UpdateScoreTexts()

func UpdateScoreTexts() -> void:
	
	$Control/PlayerScore.text = "Score: " + str(playerScore)
	$Control/BotScore1.text = "Score: " + str(bot1Score)
	$Control/BotScore2.text = "Score: " + str(bot2Score)
	$Control/BotScore3.text = "Score: " + str(bot3Score)



func Reset() -> void:
	for container in get_tree().get_nodes_in_group("Container"):
		container.Reset()
		
	_ready()
