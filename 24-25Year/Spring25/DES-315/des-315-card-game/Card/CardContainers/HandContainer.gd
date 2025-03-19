class_name HandContainer
extends CardContainer


var hand : Dictionary[CardBase, bool]


@export var playerHand : bool = false


@export var spacing := 100.0
@export var angle := 30.0
@export var height := 50.0


@export_category("CardHovering")
@export var manuallyPlayable : bool = false
@export var hoverSize : float = 2.5
@export var hoverHeight : float = 50.0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta * actionSpeed)


func HandArrangement(numCards : int) -> Array[Transform2D]:
	var layout : Array[Transform2D] = []
	
	if numCards == 0:
		return layout
	
	var width : float = (numCards - 1) * spacing
	var center : float  = width / 2
	
	
	var horAxis := transform.x.normalized()
	
	var vertAxis := transform.y.normalized()
	
	for i in range(numCards):
		var xPos = i * spacing - center
		
		var horOffset := i * spacing - center
		
		var normalized: float = 0.0
		if width > 0.0:
			normalized = abs(horOffset) / width
		else:
			normalized = 0.0
		
		var vertOffset := -height * (1.0 - pow(normalized, 2))
		
		
		var cardPos := global_position + (horOffset * horAxis)
		
		cardPos = cardPos + (vertOffset * vertAxis)
		
		var curAngle = -angle/2
		
		if numCards > 1:
			curAngle += angle * i / (numCards - 1)
		
		var newTransform := Transform2D(deg_to_rad(curAngle + rotation_degrees), cardPos)
		
		layout.push_back(newTransform)
	
	return layout



func GetCardHandPos(card: CardBase) -> Transform2D:
	var cardIndex := 0
	
	for cards in hand.keys():
		if cards == card:
			break
		cardIndex += 1
		
	var layouts = HandArrangement(hand.keys().size())
	
	return layouts[cardIndex]

func GetDealPosition() -> Transform2D:
	var newLayout = HandArrangement(hand.size() + 1)
	
	return newLayout.back()


func AddCard(card : CardBase) -> void:
	hand[card] = false
	
	var cardIndex = hand.keys().size() - 1
	
	card.z_index = cardIndex
	
	if playerHand:
		card.ShowFace()
		
	card.mouseEntered.connect(CardHovered)
	card.mouseExited.connect(CardUnhovered)
	card.clicked.connect(CardClicked)

func RemoveCard(card : CardBase) -> void:
	hand.erase(card)
	card.mouseEntered.disconnect(CardHovered)
	card.mouseExited.disconnect(CardUnhovered)
	card.clicked.disconnect(CardClicked)

func GetRandom() -> CardBase:
	return hand.keys().pick_random()

func CardHovered(card: CardBase) -> void:
	
	if hand[card] == true:
		return
		
	#var cardIndex = hand.keys().find(card)
	#
	#var nextCard : CardBase = null
	#
	#if cardIndex + 1 < hand.keys().size():
		#nextCard = hand.keys()[cardIndex + 1]
	#
	#if nextCard != null and hand[nextCard] == true:
		#return
	
	hand[card] = true
	
	var cardY = -card.transform.y.normalized()
	
	var easingMethod = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var newCardPos = GetCardHandPos(card).get_origin() + (cardY * hoverHeight)
	
	var translateAction := TranslateAction.new(false, false, "CardHover" + card.name, 0.5, 0.0, false, card, newCardPos, easingMethod)
	
	var easingMethod2 = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	
	var scaleAction := ScaleAction.new(true, false, "CardHover" + card.name, 0.5, 0.0, false, card, Vector2(hoverSize, hoverSize), easingMethod2)
	
	actionList.PushBack(translateAction)
	actionList.PushBack(scaleAction)
	
func CardUnhovered(card: CardBase) -> void:
	
	if hand[card] == false:
		return
		
	hand[card] = false
	
	var cardY = -card.transform.y.normalized()
	
	var easingMethod = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var newCardPos = GetCardHandPos(card).get_origin()
	
	var translateAction := TranslateAction.new(false, false, "CardHover" + card.name, 0.5, 0.0, false, card, newCardPos, easingMethod)
	
	var easingMethod2 = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	
	var scaleAction := ScaleAction.new(true, false, "CardHover" + card.name, 0.5, 0.0, false, card, Vector2(2.0, 2.0), easingMethod2)
	
	actionList.PushBack(translateAction)
	actionList.PushBack(scaleAction)


func CardClicked(card: CardBase) -> void:
	if !manuallyPlayable:
		return
	
	var easingMethod2 = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	
	var scaleAction := ScaleAction.new(true, false, "CardHover" + card.name, 0.5, 0.0, false, card, Vector2(2.0, 2.0), easingMethod2)
		
		
	actionList.PushBack(scaleAction)
	
	
	var gameBoard = get_tree().get_first_node_in_group("GameController") as GameBoard
	
	gameBoard.PlayCards(card)
	
	
func Reset() -> void:
	for card in hand.keys():
		card.queue_free()
		
	hand.clear()
