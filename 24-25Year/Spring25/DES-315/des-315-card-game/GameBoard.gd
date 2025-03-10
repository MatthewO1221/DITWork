class_name GameBoard
extends Node2D

var delay := 0.1


@export var handSize := 7
@export var playSpeed := 1.0
@export var botNum := 3

@onready var deck = $DeckContainer
@onready var hand = $PlayerHand

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

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	deck.SpawnDeck()
	ShuffleDeck()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta * playSpeed)


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("Deal"):
		DealHands()
	if event.is_action_pressed("Shuffle"):
		ShuffleDeck()
	if event.is_action_pressed("FlipHand"):
		FlipHand()
	if event.is_action_pressed("Pause"):
		Pause()
	if event.is_action_pressed("Play Cards"):
		PlayCards()

func DealHands() -> void:
	
	var fakeHands = get_tree().get_nodes_in_group("HandContainer")
	
	var hands : Array[HandContainer]
	
	for hand in fakeHands:
		hands.push_back(hand as HandContainer)
		
	var dealCurve := CustomCurve.new(Tween.TransitionType.TRANS_CUBIC, Tween.EaseType.EASE_OUT)
	
	var dealAction = DealHandsAction.new(false, false, "Deal", 0.5, 0.0, false, hands, handSize, deck, 0.1, dealCurve)
	
	actionList.PushBack(dealAction)

func ShuffleDeck() -> void:
	
	var shuffleCurve := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN)
	
	var shuffleAction = ShuffleAction.new(true, false, "Shuffle", 1.0, 0.0, false, deck, shuffleCurve)
	
	actionList.PushBack(shuffleAction)
	
func FlipHand() -> void:
	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN)
	
	for card in hand.hand:
		var flipAction = FlipCardAction.new(true, false, "HandFlip", 1.0, 0.0, false, card, easingMethod)
		actionList.PushBack(flipAction)

func PlayCards() -> void:
	var trickContainer = $TrickContainer
	
	var fakeHands = get_tree().get_nodes_in_group("HandContainer")
	
	var hands : Array[HandContainer]
	
	for hand in fakeHands:
		hands.push_back(hand as HandContainer)
	
	trickContainer.GrabCards(hands)

func Pause() -> void:
	var pauseMenuResource = load("res://PauseScene/PauseMenu.tscn")

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
