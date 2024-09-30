#include "AEEngine.h"

struct Level;
struct World;

typedef int SERIALIZEMODE;
typedef int FILETYPE;
typedef int PICKERRESULT;

#define SERIALIZEMODE_NEW 0
#define SERIALIZEMODE_SAVE 1
#define SERIALIZEMODE_LOAD 2

#define FILETYPE_LEVEL 0
#define FILETYPE_WORLD 1

#define PICKERRESULT_FAIL -1
#define PICKERRESULT_CANCEL 0
#define PICKERRESULT_SUCCESS 1

// Creates a new level based on file picker location.
int NewLevelWithPicker(const struct Level*, const struct Tilemap*, char[]);

// Saves a level based on file picker location.
int SaveLevelWithPicker(const struct Level*, const struct Tilemap*);

// Saves a level to a path.
int SaveLevelAtPath(const struct Level*, const struct Tilemap*, const char*);

// Loads a level based on file picker location.
int LoadLevelWithPicker(struct Level*, struct Tilemap*);

// Loads a level from a path.
int LoadLevelAtPath(struct Level*, struct Tilemap*, const char*);

// Creates a new world based on file picker location.
int NewWorldWithPicker(const struct World*, char[]);

// Saves a world based on file picker location.
int SaveWorldWithPicker(const struct World*);

// Saves a world to a path.
int SaveWorldAtPath(const struct World*, const char*);

// Loads a world based on file picker location.
int LoadWorldWithPicker(struct World*);

// Loads a world from a path.
int LoadWorldAtPath(struct World*, const char*);

// Saves the player's information.
int SavePlayerData(const char*);

// Loads the saved player information.
int LoadPlayerData(const char*);

// Saves the current settings.
int SaveOptionsData(const char*);

// Loads the saved settings.
int LoadOptionsData(const char*);

// Saves the boss data.
int SaveBossData(const char*);

// Loads the boss data.
int LoadBossData(const char*);

// Saves the collectible settings.
int SaveCollectibleData(const char*);

// Loads the collectible settings.
int LoadCollectibleData(const char*);

// Sets all settings and data back to the default (resets the game!)
int ResetAllData(void);

// Saves all data.
int SaveAllData(void);

// Returns the local path for storing save data.
const char* GetLocalPath(void);

// Returns whether or not a file exists.
int DoesFileExist(const char*);

float GetSaveTimeAlpha(void);

void SetSaveTimeAlpha(float);
