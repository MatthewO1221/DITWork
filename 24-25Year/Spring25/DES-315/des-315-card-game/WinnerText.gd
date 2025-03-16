class_name WinnerText
extends Label





var actionList := ActionList.new()


func _process(delta: float) -> void:
	actionList.UpdateAllActions(delta)



func win(winners: Dictionary[HandContainer, CardBase]) -> void:
	text = ""
	for winner in winners.keys():
		winner = winner as HandContainer
		text += winner.name + " wins with a(n): \n"
		
		var card = winners[winner]
		
		var cardValue = card.valueMap[card.value]
		
		var cardSuit = card.suitMap[card.suit]
		
		text += cardValue + " of " + cardSuit + "\n"

	var easingMethod := CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var fadeInAction := FadeAction.new(true, false, "WinText", 3.0, 0.0, false, self, 1.0, easingMethod)
	
	var easingMethod2 = CustomCurve.new(Tween.TransitionType.TRANS_LINEAR)
	
	var fadeOutAction := FadeAction.new(false, false, "WinText", 3.0, 0.0, false, self, 0.0, easingMethod2)
	
	actionList.PushBack(fadeInAction)
	
	actionList.PushBack(fadeOutAction)
