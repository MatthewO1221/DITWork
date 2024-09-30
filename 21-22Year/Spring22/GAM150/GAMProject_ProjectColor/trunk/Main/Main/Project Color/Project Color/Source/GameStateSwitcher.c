#include "stdafx.h"		// Should be included first in every .c file!
#include "GameStateSwitcher.h"
#include "GameStateManager.h"

void ToggleGameState(void)
{
  if (AEInputCheckTriggered(VK_OEM_3)) // the tilde ('~') key
    GameStateManagerSetNextState(GsMenuTest);
}
