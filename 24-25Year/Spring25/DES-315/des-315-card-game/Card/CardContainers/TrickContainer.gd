class_name TrickContainer
extends CardContainer


var cards : Dictionary[HandContainer, CardBase]


@export var spacing := 100.0
@export var height := 50.0

@export var discard : DiscardContainer



var scoreTimer := Timer.new()

@export var scoreTime := 5.0

var scoring : bool = false

var gameController : GameBoard = null

signal DoneScoring

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	gameController = get_tree().get_first_node_in_group("GameController")
	
	add_child(scoreTimer)
	
	scoreTimer.process_callback = Timer.TIMER_PROCESS_PHYSICS
	
	scoreTimer.timeout.connect(Score)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	if !actionList.IsEmpty():
		actionList.UpdateAllActions(delta)
		
		
		
		
		
func Score() -> void:
	var winners = GetWinningCard()
	gameController.AddScore(winners)
	scoring = false
	discard.GrabCards(self)
	cards.clear()
	DoneScoring.emit()

func CardArrangement(numCards : int) -> Array[Transform2D]:
	var layout : Array[Transform2D] = []
	
	if numCards == 0:
		return layout
	
	var width : float = (numCards - 1) * spacing
	var center : float  = width / 2
	
	
	var horAxis := global_transform.x.normalized()
	
	var vertAxis := global_transform.y.normalized()
	
	for i in range(numCards):
		var xPos = i * spacing - center
		
		var horOffset := i * spacing - center
		
		var normalized: float = 0.0
		if width > 0.0:
			normalized = abs(horOffset) / width
		else:
			normalized = 0.0
		
		var vertOffset := 0.0 #-height * (1.0 - pow(normalized, 2))
		
		
		var cardPos := global_position + (horOffset * horAxis)
		
		cardPos = cardPos + (vertOffset * vertAxis)
		
		
		
		var newTransform := Transform2D(0.0, cardPos)
		
		layout.push_back(newTransform)
	
	return layout

	
func GetDealPosition() -> Transform2D:
	var newLayout = CardArrangement(cards.size() + 1)
	
	return newLayout.back()


func AddCard(hand: HandContainer, card : CardBase) -> void:
	cards[hand] = card
	

func RemoveCard(card : CardBase) -> void:
	cards.erase(card)

func GrabCards(hands: Array[HandContainer]) -> void:
	
	for hand in hands:
		var card = hand.GetRandom()
		#print(card.valueMap[card.value] + card.suitMap[card.suit])
		hand.RemoveCard(card)
		AddCard(hand, card)
		
	var layouts := CardArrangement(cards.size())
	
	for i in cards.size():
		var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_SPRING, Tween.EaseType.EASE_OUT)
		
		var moveAction := TranslateAction.new(false, false, "GrabCards", 1.0, 0.0, false, cards[cards.keys()[i]], layouts[i].origin, easingMethod)
		
		actionList.PushBack(moveAction)
		
		if i != 0:
		
			var easingMethod2 := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR, Tween.EaseType.EASE_IN_OUT)
		
			var flipAction := FlipCardAction.new(false, false, "GrabCards", 0.25, 0.0, false, cards[cards.keys()[i]], easingMethod2)
		
			actionList.PushBack(flipAction)
		
		
		
		
		var easingMethod3 := CustomCurve.new(Tween.TransitionType.TRANS_SPRING, Tween.EaseType.EASE_OUT)
		
		var rotateAction := RotateAction.new(true, false, "GrabCards", 1.0, 0.0, false, cards[cards.keys()[i]], rad_to_deg(layouts[i].get_rotation()), easingMethod3)
		
		actionList.PushBack(rotateAction)
		
		
		var easingMethod4 := CustomCurve.new(Tween.TransitionType.TRANS_SPRING, Tween.EaseType.EASE_IN_OUT)
		
		var arrangeAction := ArrangeHandAction.new(false, false, "ArrangeHand", 2.0, 0.0, false, cards.keys()[i], easingMethod4)
		
		cards.keys()[i].actionList.PushBack(arrangeAction)
		
	scoreTimer.start(scoreTime)


func GrabCard(hand: HandContainer, card: CardBase) -> void:
	hand.RemoveCard(card)
	AddCard(hand, card)
	
	
	
	
func GetWinningCard() -> Dictionary[HandContainer, CardBase]:
	
	if Automator.curState != Automator.AutomationStates.Inactive:
		Telemeter.CardsPlayed(cards.values())
	
	
	var highestValue : int = 0
	
	for card in cards.values():
		if card.value > highestValue:
			highestValue = card.value
	
	var winningCards : Dictionary[HandContainer, CardBase]
	
	for key in cards.keys():
		if cards[key].value == highestValue:
			winningCards[key] = cards[key]
			
	return winningCards


func Reset() -> void:
	for card in cards.values():
		card.queue_free()
		
	cards.clear()
