//------------------------------------------------------------------------------
//
// File Name:	Button.h
// Author(s):	Brody Endstrasser
// Project:		Project Color
// Course:		GAM150B
//
// Copyright © 2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "AEEngine.h"
#include "Vector2D.h"
#include "AABB.h"
#include "GameObject.h"

#define MAX_BUTTONS_ONSCREEN 10
#define BUTTON_WIDTH 75 * 4//64 * 4 //pixels
#define BUTTON_HEIGHT 18 * 4 //16 * 4//pixels

//y'know it's just hit me that with this struct we are essentially trying to recreate classes in c lmao
struct ButtonStruct 
{
    GameObjectPtr gme;
    //center of the button
    Vector2D pos;
    //col-> world pos is top left 
    AABB col;
    AEGfxVertexList* buttMesh;
    AEGfxTexture* buttTexture;
    int isSelected, // is the button the currently selected one, based on arrow key input, would also be TRUE if isHovered is TRUE
        isHovered, //if the mouse is over the button
        isDown; // button is being held down
    int id;
    char * label;
    GameObjectPtr textobj;

    void (*onTriggered)(Button), // called once, as soon as the button is down (down sprite)
         (*onDown)(Button), // call this every frame the is being held down, but mouse hasn't lifted (down sprite, any code to do while button down)
         (*onRelease)(Button), // button was previously held down, but has been released (select sprite, code to switch gamestates)
         (*onSelect)(Button), // called once as soon as the button is hovered by the mouse or selected with the arrow keys (select sprite)
         (*onDeselect)(Button); // called once as soon as the button is no longer selected and/or hovered (default sprite)
};

typedef struct ButtonStruct* Button;

enum buttSprite 
{
    contDefault,
    nGameDefault,
    setDefault,
    creditsDefault,
    quitDefault,
    contSelect,
    nGameSelect,
    setSelect,
    creditsSelect,
    quitSelect,
    contDown,
    nGameDown,
    setDown,
    creditsDown,
    quitDown,
};


int ButtonsInUse();

Button ButtonCreate(Vector2D topl, float width, float height, char* label);

void ButtonUpdate(Button button);

void ButtonDestroy(Button button);

void ButtonAssignFunctions(Button b);

void ButtonDraw(Button button);

void textInit();

//static variables are weird
Button getButt(int index);

void onTriggered_play(Button b);
void onDown_play(Button b);
void onRelease_play(Button b);
void onSelect_play(Button b);
void onDeselect_play(Button b);


void onTriggered_quit(Button b);
void onDown_quit(Button b);
void onRelease_quit(Button b);
void onSelect_quit(Button b);
void onDeselect_quit(Button b);

/// "New Game"--------------------------------------------------
void onTriggered_ngame(Button b);
void onDown_ngame(Button b);
void onRelease_ngame(Button b);
void onSelect_ngame(Button b);
void onDeselect_ngame(Button b);
/// "Settings"--------------------------------------------------
void onTriggered_set(Button b);
void onDown_set(Button b);
void onRelease_set(Button b);
void onSelect_set(Button b);
void onDeselect_set(Button b);
/// "Credits"--------------------------------------------------
void onTriggered_credits(Button b);
void onDown_credits(Button b);
void onRelease_credits(Button b);
void onSelect_credits(Button b);
void onDeselect_credits(Button b);