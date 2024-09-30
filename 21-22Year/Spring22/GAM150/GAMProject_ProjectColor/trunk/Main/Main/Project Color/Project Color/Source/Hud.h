#pragma once

void LoadStatusBar();

void InitStatusBar();

void UpdateAndDrawStatusBar(char health, char maxHealth, int gems, int paintColor, float paintAmount, float maxPaint, float dt);

void DisplaySaveMessage(float alpha);

void ShutdownStatusBar();

void UnloadStatusBar();
