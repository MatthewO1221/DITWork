extends Node


## Automator works off state machine, these are all the states
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

## The game controller
var gameBoard : GameBoard

## The pause menu
var pauseMenu : PauseMenu

## The play speed slider in pause menu
var playSpeed: Slider


## The current state of the automator
var curState : AutomationStates = AutomationStates.Inactive

## Whether the automator is awaiting a signal before continuing
var awaiting : bool = false

func _ready() -> void:
	gameBoard = get_tree().get_first_node_in_group("GameController") as GameBoard
	
	pauseMenu = get_tree().get_first_node_in_group("PauseMenu") as PauseMenu
	
	for child in pauseMenu.get_children()[0].get_children():
		if child.name == "PlaySpeed":
			playSpeed = child as Slider
	
	# Automator should work even while paused
	process_mode = Node.PROCESS_MODE_ALWAYS

func _process(delta: float) -> void:
	# If not waiting for a signal automate
	if !awaiting:
		Automate(delta)
	
	


func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("Automate"):
		HandleInput()

## Handles automation request
func HandleInput() -> void:
	
	# If automator inactive start it, otherwise stop it
	match curState:
		AutomationStates.Inactive:
			curState = AutomationStates.Starting
		_:
			curState = AutomationStates.Stopping
			
## When automator moves from inactive to starting
func StartAutomating() -> void:
	playSpeed.value = 5
	
	pauseMenu.UpdatePlaySpeed(true)
	
## When automator moves from any state to stopping
func StopAutomating() -> void:
	playSpeed.value = 1
	
	pauseMenu.UpdatePlaySpeed(true)
	
## Change the state of the automator
func ChangeState(newState: AutomationStates) -> void:
	curState = newState

## Handles frame by frame logic for automator, contains function calls for state machine
func Automate(delta: float) -> void:
	match curState:
		AutomationStates.Inactive:
			return
		AutomationStates.Starting:
			StartAutomating()
			ChangeState(AutomationStates.Dealing)
		AutomationStates.Playing:
			# Log that a new hand is being played
			Telemeter.NewHand(gameBoard.handSize, gameBoard.botNum)
			# Play a hand and wait for it to be done
			gameBoard.PlayHand()
			awaiting = true
			await gameBoard.DonePlaying
			awaiting = false
			# Log the scores after the hand is done playing
			Telemeter.EndScores([gameBoard.playerScore, gameBoard.bot1Score, gameBoard.bot2Score, gameBoard.bot3Score])
			ChangeState(AutomationStates.Resetting)
		AutomationStates.Pausing:
			# Pause the game and wait for the menu to be in place
			pauseMenu.Pause()
			awaiting = true
			await get_tree().create_timer(1.0).timeout
			awaiting = false
			ChangeState(AutomationStates.Changing)
		AutomationStates.Changing:
			# Change a random game value
			ChangeGameValue()
			ChangeState(AutomationStates.Unpausing)
		AutomationStates.Resetting:
			# Reset the game board after every hand played
			gameBoard.Reset()
			ChangeState(AutomationStates.Pausing)
		AutomationStates.Unpausing:
			# Unpause the game
			pauseMenu.Unpause()
			awaiting = true
			await get_tree().create_timer(1.0).timeout
			awaiting = false
			ChangeState(AutomationStates.Dealing)
		AutomationStates.Dealing:
			# Deal hands
			gameBoard.DealHands()
			awaiting = true
			await get_tree().create_timer(5.2).timeout
			awaiting = false
			ChangeState(AutomationStates.Playing)

## Asigns a random value to either hand size or hand number
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
