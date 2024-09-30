//------------------------------------------------------------------------------
//
// File Name:	Button.c
// Author(s):	Brody Endstrasser
// Project:		Project Color
// Course:		GAM150B
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "AEEngine.h"
#include "Button.h"
#include "Vector2D.h"
#include "GameStateSwitcher.h"
#include "GameStateManager.h"
#include "Sprite.h"
#include "SpriteSource.h"
#include "Audio.h"
#include "Serialization.h"
#include "PauseScreen.h"
#include "Camera.h"

//contains the adress of each button that exists
//button->id is the index of this array
//to derefrence a specific button, use ButtList[id]
static Button ButtList[MAX_BUTTONS_ONSCREEN] =
{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

static AEGfxVertexList* mesh16x6fontSheet;
static AEGfxTexture* fontTexture;

int ButtonsInUse()
{
    for (int i = 0; i < MAX_BUTTONS_ONSCREEN; i++) {
        if (ButtList[i] == NULL)
            return i;
    }

    return 0;
}

void textInit()
{
    mesh16x6fontSheet = MeshCreateQuad(.5f, .5f, 1.f / 16.f, 1.f / 6.f, "mesh16x6fontSheet");
    fontTexture = AEGfxTextureLoad("./Assets/font.png");
}

Button ButtonCreate(Vector2D topl, float width, float height, char* label)
{

    Button newButton = malloc(sizeof(struct ButtonStruct));

    if (newButton) {
        Vector2D botLeft;
        Vector2D subVec = { width / 2, height / 2 };
        Vector2DSub(&botLeft, &topl, &subVec);

        newButton->pos = topl;
        newButton->col = FromPoints(botLeft, width, height, 0, 0, false);

        newButton->buttTexture = AEGfxTextureLoad("./Assets/buttons/butt_sheet2.png");

        newButton->gme = GameObjectCreate();
        GameObjectAddSprite(newButton->gme, SpriteCreate());

        TransformPtr transform = TransformCreate();
        GameObjectAddTransform(newButton->gme, transform);
        Vector2D scale = { width, height };
        TransformSetTranslation(transform, &topl);
        TransformSetScale(transform, &scale);
        SpriteSetSpriteSource(GameObjectGetSprite(newButton->gme), SpriteSourceCreate(5, 3, newButton->buttTexture));

        //allocate a char array using size of label & copy it in
        const int labelSize = strlen(label) + 1;
        newButton->label = malloc(sizeof(char) * labelSize);
        if (newButton->label) {
            strcpy_s(newButton->label, labelSize, label);
            //don't forget null pointer
            //newButton->label[strlen(label)] = '\0';
        }

        //if there are more than the max amount of buttons onscreen, replace the first button with the new one
        //do we really need more than 10 buttons onscreen at once?
        if (ButtonsInUse() > MAX_BUTTONS_ONSCREEN) {
            ButtonDestroy(ButtList[0]);
            newButton->id = 0;
        }
        else
            newButton->id = ButtonsInUse();

        ButtList[newButton->id] = newButton;

        ButtonAssignFunctions(newButton);

        newButton->buttMesh = MeshCreateQuad(.5f, .5f, 1.0f / 5, 1.0f / 3, label);
        //newButton->buttTexture = NULL;

        SpritePtr sprite = GameObjectGetSprite(newButton->gme);
        SpriteSetMesh(sprite, newButton->buttMesh);

        //set default texture of unselected button
        //newButton->buttTexture = AEGfxTextureSet(texturename, 1,1);

        newButton->textobj = GameObjectCreate();


        return newButton;
    }
    else
        return NULL;
  
}

void ButtonAssignFunctions(Button b)
{

    if (strcmp(b->label, "Play") == 0) {
        b->onTriggered = onTriggered_play;
        b->onDown = &onDown_play;
        b->onRelease = &onRelease_play;
        b->onSelect = &onSelect_play;
        b->onDeselect = &onDeselect_play;
    }
    else if (strcmp(b->label, "Quit") == 0) {
        b->onTriggered = &onTriggered_quit;
        b->onDown = &onDown_quit;
        b->onRelease = &onRelease_quit;
        b->onSelect = &onSelect_quit;
        b->onDeselect = &onDeselect_quit;
    }
    else if (strcmp(b->label, "New Game") == 0) {
        b->onTriggered = &onTriggered_ngame;
        b->onDown = &onDown_ngame;
        b->onRelease = &onRelease_ngame;
        b->onSelect = &onSelect_ngame;
        b->onDeselect = &onDeselect_ngame;
    }
    else if (strcmp(b->label, "Settings") == 0) {
        b->onTriggered = &onTriggered_set;
        b->onDown = &onDown_set;
        b->onRelease = &onRelease_set;
        b->onSelect = &onSelect_set;
        b->onDeselect = &onDeselect_set;
    }
    else if (strcmp(b->label, "Credits") == 0) {
        b->onTriggered = onTriggered_credits;
        b->onDown = onDown_credits;
        b->onRelease = onRelease_credits;
        b->onSelect = onSelect_credits;
        b->onDeselect = onDeselect_credits;
    }
    else
        AESysPrintf("ERROR:incompatable button id. someone messed up & i bet it was andrew lmao");
}      

void ButtonUpdate(Button button)
{
    s32 mouseX, mouseY;
    Vector2D mouseWorldPos;

    AEInputGetCursorPosition(&mouseX, &mouseY);
    AEGfxConvertScreenCoordinatesToWorld((float)mouseX, (float)mouseY, &mouseWorldPos.x, &mouseWorldPos.y);

    /*Vector2D cursorXY;
    cursorXY.x = (int)mouseX;
    cursorXY.y = (int)mouseY;*/

    if (AABBPointContained(button->col, (float)mouseWorldPos.x, (float)mouseWorldPos.y)) {
        // we know the button is being hovered at this point
        button->isHovered = true;
        button->isSelected = true;
        // do something to indicate the button is being hovered (brighten the button's background and/or text)
        button->onSelect(button);
        
        //if the button is currently down
        if (AEInputCheckCurr(VK_LBUTTON)) {

            //if the button was previously up, then call ontriggered
            if (button->isDown == false) {
                button->onTriggered(button);
            }

            button->isDown = true;
            button->onDown(button);
        }

        //if the button is released this frame
        //half an a press
        if (AEInputCheckReleased(VK_LBUTTON)) {
            button->isHovered = false;
            button->isSelected = true;
            button->isDown = false;

            button->onRelease(button);

        }

    }
    else {
        if (button->isHovered) {
            
            button->onDeselect(button);
        }
        button->isHovered = false;
        button->isDown = false;
    }

    ButtonDraw(button);
        
}

void ButtonDraw(Button button)
{
    if (button) {
        SpriteDraw(GameObjectGetSprite(button->gme), GameObjectGetTransform(button->gme));
        //AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        //AEGfxTextureSet(button->buttTexture, 0, 0);
        //AEGfxSetPosition(button->pos.x, button->pos.y);
        //// Drawing the mesh (list of triangles)
        //AEGfxSetTransparency(1.0f);
        //AEGfxMeshDraw(button->buttMesh, AE_GFX_MDM_TRIANGLES);


    }
}

void ButtonDestroy(Button button)
{
    if (button) {
        if (button->label)
            free(button->label);

        if (button->buttTexture != NULL)
            AEGfxTextureUnload(button->buttTexture);

        if (button->buttMesh != NULL)
            AEGfxMeshFree(button->buttMesh);

        if (button->gme != NULL)
            GameObjectFree(&button->gme);

        if (button->textobj != NULL)
            GameObjectFree(&button->textobj);

        ButtList[button->id] = NULL;

        free(button);
    }

}

Button getButt(int index)
{
    return ButtList[index];
}

/// "Play"--------------------------------------------------
void onTriggered_play(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, contDown); //down sprite
    PlaySFX("./Assets/SFX/button_down.mp3");
}
void onDown_play(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, contDown);//down sprite
}
void onRelease_play(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, contSelect);//select sprite
    PlaySFX("./Assets/SFX/button_up.mp3");
    
    if (GetCurrentGS() == GsTitle)
        GameStateManagerSetNextState(GsMain);
    else
        PauseToggle();

}
void onSelect_play(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, contSelect);//select sprite
}
void onDeselect_play(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, contDefault);//default
}
/// "Quit"--------------------------------------------------
void onTriggered_quit(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, quitDown);//down
    PlaySFX("./Assets/SFX/button_down.mp3");

}
void onDown_quit(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, quitDown);//down
}
void onRelease_quit(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, quitSelect);
    PlaySFX("./Assets/SFX/button_up.mp3");
    if (GetCurrentGS() == GsTitle)
        GameStateManagerSetNextState(GsQuit);//select
    else {
        PauseToggle();
        GameStateManagerSetNextState(GsTitle);
    }
}
void onSelect_quit(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, quitSelect);//select
}
void onDeselect_quit(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, quitDefault);//default
}
/// "New Game"--------------------------------------------------
void onTriggered_ngame(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, nGameDown); //down sprite
    PlaySFX("./Assets/SFX/button_down.mp3");
}
void onDown_ngame(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, nGameDown);//down
}
void onRelease_ngame(Button b) //WHERE THE CODE GOES
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, nGameSelect);//select
    PlaySFX("./Assets/SFX/button_up.mp3");
    ResetAllData();
    GameStateManagerSetNextState(GsMain);
}
void onSelect_ngame(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, nGameSelect);//select
}
void onDeselect_ngame(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, nGameDefault);//default
}
/// "Settings"--------------------------------------------------
void onTriggered_set(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, setDown); //down sprite
    PlaySFX("./Assets/SFX/button_down.mp3");
}
void onDown_set(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, setDown);//down
}
void onRelease_set(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, setSelect);//select
    PlaySFX("./Assets/SFX/button_up.mp3");
    GameStateManagerSetNextState(GsMenuTest);
}
void onSelect_set(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, setSelect);//select
}
void onDeselect_set(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, setDefault);//default
}
/// "Credits"--------------------------------------------------
void onTriggered_credits(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, creditsDown); //down sprite
    PlaySFX("./Assets/SFX/button_down.mp3");
}
void onDown_credits(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, creditsDown);//down
}
void onRelease_credits(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, creditsSelect);//select
    PlaySFX("./Assets/SFX/button_up.mp3");
}
void onSelect_credits(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, creditsSelect);//select
}
void onDeselect_credits(Button b)
{
    SpritePtr sprite = GameObjectGetSprite(b->gme);
    SpriteSetFrame(sprite, creditsDefault);//default
}