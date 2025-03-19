extends Node



enum AutomationStates
{
	Inactive,
	Starting,
	Dealing,
	Playing,
	Pausing,
	Unpausing,
	Changing,
	Resetting,
	Stopping
}


var gameBoard : GameBoard

var pauseMenu : PauseMenu

var playSpeed: Slider



var curState : AutomationStates = AutomationStates.Inactive

var awaiting : bool = false

func _ready() -> void:
	gameBoard = get_tree().get_first_node_in_group("GameController") as GameBoard
	
	pauseMenu = get_tree().get_first_node_in_group("PauseMenu") as PauseMenu
	
	for child in pauseMenu.get_children()[0].get_children():
		if child.name == "PlaySpeed":
			playSpeed = child as Slider
			
	process_mode = Node.PROCESS_MODE_ALWAYS

func _process(delta: float) -> void:
	if !awaiting:
		Automate(delta)
	
	


func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("Automate"):
		HandleInput()


func HandleInput() -> void:
	match curState:
		AutomationStates.Inactive:
			curState = AutomationStates.Starting
		_:
			curState = AutomationStates.Stopping
			
			
func StartAutomating() -> void:
	playSpeed.value = 5
	
	pauseMenu.UpdatePlaySpeed(true)
	
func StopAutomating() -> void:
	playSpeed.value = 1
	
	pauseMenu.UpdatePlaySpeed(true)
	
func ChangeState(newState: AutomationStates) -> void:
	curState = newState

func Automate(delta: float) -> void:
	match curState:
		AutomationStates.Inactive:
			return
		AutomationStates.Starting:
			StartAutomating()
			ChangeState(AutomationStates.Dealing)
		AutomationStates.Playing:
			Telemeter.NewHand(gameBoard.handSize, gameBoard.botNum)
			gameBoard.PlayHand()
			awaiting = true
			await gameBoard.DonePlaying
			awaiting = false
			Telemeter.EndScores([gameBoard.playerScore, gameBoard.bot1Score, gameBoard.bot2Score, gameBoard.bot3Score])
			ChangeState(AutomationStates.Resetting)
		AutomationStates.Pausing:
			pauseMenu.Pause()
			awaiting = true
			await get_tree().create_timer(1.0).timeout
			awaiting = false
			ChangeState(AutomationStates.Changing)
		AutomationStates.Changing:
			ChangeGameValue()
			ChangeState(AutomationStates.Unpausing)
		AutomationStates.Resetting:
			gameBoard.Reset()
			ChangeState(AutomationStates.Pausing)
		AutomationStates.Unpausing:
			pauseMenu.Unpause()
			awaiting = true
			await get_tree().create_timer(1.0).timeout
			awaiting = false
			ChangeState(AutomationStates.Dealing)
		AutomationStates.Dealing:
			gameBoard.DealHands()
			awaiting = true
			await get_tree().create_timer(5.2).timeout
			awaiting = false
			ChangeState(AutomationStates.Playing)

func ChangeGameValue() -> void:
	var rand = randi_range(0, 1)
	
	var slider : Slider
	
	if rand == 0:
		for child in pauseMenu.get_children()[0].get_children():
			if child.name == "HandSize":
				slider = child
	elif rand == 1:
		for child in pauseMenu.get_children()[0].get_children():
			if child.name == "HandNumber":
				slider = child
	
	slider.value = randi_range(slider.min_value, slider.max_value)
	
	pauseMenu.UpdateHandNumber(true)
	pauseMenu.UpdateHandSize(true)
