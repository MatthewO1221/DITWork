#include "AEEngine.h"

struct Vector2D;

#define CAMERA_SPEED 5

// Initializes memory used for the camera.
void InitCamera(void);

// Updates the camera's position.
void UpdateCamera(void);

// Shutsdown and frees memory uesd by the camera.
void ShutdownCamera(void);

// Renders the camera target position.
void DrawCamera(void);

// Update camera position.
void FollowTarget(const struct Vector2D*, int);

// Returns the camera's position.
struct Vector2D GetCamPos(void);

// Sets the camera's position.
void SetCamPos(const struct Vector2D*, int);

// Sets the X position of the camera.
void SetCamX(float);

// Sets the Y position of the camera.
void SetCamY(float);

// Returns the camera's old position.
struct Vector2D GetOldCamPos(void);

// Sets the camera's old position.
void SetOldCamPos(const struct Vector2D*);

// Sets the old X position of the camera.
void SetOldCamX(float);

// Sets the old Y position of the camera.
void SetOldCamY(float);

// Returns the camera's target position.
struct Vector2D GetTargetCamPos(void);

// Sets the camera's target position.
void SetTargetCamPos(const struct Vector2D*);

// Sets the target X position of the camera.
void SetTargetCamX(float);

// Sets the target Y position of the camera.
void SetTargetCamY(float);

// Sets the camera to be clamped.
void ClampCamera(int);
