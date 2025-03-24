class_name GameBoard
extends Node2D

var delay := 0.1


@export var handSize := 7
@export var playSpeed := 1.0
@export var botNum := 3

@onready var deck = $DeckContainer
@onready var playerHand := $Player as HandContainer
@onready var botHand1 := $Bot1 as HandContainer
@onready var botHand2 := $Bot2 as HandContainer
@onready var botHand3 := $Bot3 as HandContainer

var actionList := ActionList.new()

var playerScore := 0
var bot1Score := 0
var bot2Score := 0
var bot3Score := 0

@onready var botName1 = $Control/BotLabel1
@onready var botName2 = $Control/BotLabel2
@onready var botName3 = $Control/BotLabel3
@onready var botScore1 = $Control/BotScore1
@onready var botScore2 = $Control/BotScore2
@onready var botScore3 = $Control/BotScore3


var debugMode : bool = false


signal DonePlaying

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	deck.SpawnDeck()
	ShuffleDeck()
	get_viewport().set_physics_object_picking_sort(true)
	get_viewport().set_physics_object_picking_first_only(true)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	actionList.UpdateAllActions(delta)
	
	if debugMode:
		DisplayDebugText()


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Deal"):
		DealHands()
	if event.is_action_pressed("Shuffle"):
		ShuffleDeck()
	if event.is_action_pressed("FlipHand"):
		FlipHand()
	#if event.is_action_pressed("Play Cards"):
		#PlayCards()
	if event.is_action_pressed("Debug"):
		debugMode = !debugMode
		
		if debugMode:
			$ActionDebugger.show()
		else:
			$ActionDebugger.hide()

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

func ShuffleDeck() -> void:
	
	var shuffleCurve := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN)
	
	var shuffleAction = ShuffleAction.new(true, false, "Shuffle", 1.0, 0.0, false, deck, shuffleCurve)
	
	actionList.PushBack(shuffleAction)
	
func FlipHand() -> void:
	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN)
	
	for card in playerHand.hand.keys():
		var flipAction = FlipCardAction.new(true, false, "HandFlip", 1.0, 0.0, false, card, easingMethod)
		actionList.PushBack(flipAction)

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
		var actions = actionDict[key]
		
		returnString = returnString + key + "\n"
		
		for action in actions:
			var children = action.children
			
			returnString = returnString + "\t" + action.GetActionType() + "\n"
			
			for child in children:
				var grandChildren = child.children
				
				returnString = returnString + "\t\t" + child.GetActionType() + "\n"
				
				for grandChild in grandChildren:
					returnString = returnString + "\t\t\t" + grandChild.GetActionType() + "\n"
		

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
