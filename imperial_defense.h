#ifndef __IMPERIAL_DEFENSE_H__
#define __IMPERIAL_DEFENSE_H__

#include "images.h"
#include "stdint.h"
#include <math.h>

#ifndef NULL
#define NULL           0
#endif

#define SEC_CONST      1000000
#define SEC_HALF_CONST 500000
#define START_GOLD     150
#define START_LP       15

// some game data limits
#define ENEMY_ROUTES   32
#define MAX_TURRETS    64
#define MAX_TILES      12
#define MAX_ENEMY      64
#define MAX_LEVELS     3
//TODO: get a good value for max speed
#define MAX_TURRET_SPEED  20
#define QUEUE_END         MAX_ENEMY+1
#define MAX_TURR_TYPE     3

// tile states
#define WALK_ZONE       2
#define BUILD_ZONE      0
#define TURRET_OCCUPIED 1
#define UNUSABLE_ZONE   4

//menu position states
#define TOP_MENU_BAR    1
#define MIDDLE_MENU_BAR 2
#define LOWER_MENU_BAR  3

//TODO: not sure ye but if something is wrong triple check this
// side panel positions
#define SIDE_PANEL_X      896
#define SIDE_PANEL_Y      384
#define SIDE_PANEL_HEIGHT 768 // height of screen
#define SIDE_PANEL_WIDTH  256

// game panel positions
#define GAME_PANEL_X      384
#define GAME_PANEL_Y      384
#define GAME_PANEL_HEIGHT 768
#define GAME_PANEL_WIDTH  768

//grid size -> NOTE: hardcoded
#define TILE_PIXEL_SIZE 64

//mouseLogic options
#define FIRST_OPTION  0
#define SECOND_OPTION 1
#define THIRD_OPTION  2

// turret names and defines
#define ARCHER        0
#define CANON         1
#define MAGE          2
#define NONE          3

// upgradeTurret defines
#define UPGRADE_RANGE_AMOUNT  20
#define PIXELS_PER_UPGRADE    10 // to increase price per this amount of pixels for range
#define UPGRADE_SPEED_AMOUNT  1
#define UPGRADE_DAMAGE_AMOUNT 1
#define UPGRADE_COST          30
#define DEMOLISH_GOLD         5
#define KILL_LOOT             5
#define BUILD_COST            50 // set multipler depending on tower

#define SPEED_MASK  0
#define RANGE_MASK  1
#define DAMAGE_MASK 2

#define SEC_BETWEEN_SPAWNS    3
#define SPAWN_DELAY_TIMER     10
#define ENEMY_INIT_SPEED      32
#define ENEMY_INIT_HEALTH     35
#define ENEMY_HEALTH_INCREASE 7

#define GAME_INMENU     0
#define GAME_LEVELPICK  1
#define GAME_INGAME     2
#define GAME_CREDITS    3

#define MAX_PROJECTILES   64
#define PROJECTILE_SPEED  400

#define MAX_BLOOD_ANIMS 100

typedef struct pixelpoint {
  int32_t x,y; //4 *2
} pixelPoint_t; //8

typedef struct gridpoint {
  int32_t x,y; //4 *2
} gridPoint_t;

typedef struct enemyStats {
  int32_t health; //4
  int32_t speed; //4
  pixelPoint_t location; //8
} enemyStats_t; //16

typedef struct enemy {
  int32_t currentRoute; //point to the route enemy is aiming to //4
  enemyStats_t *stats; //8
  int32_t spriteLocation; //4
  int32_t uniqueID; //4
} enemy_t; //20 --> might be an error

// Map grid Entities

typedef struct enemNode {
  enemy_t *enemy; // pointer = 4
  struct enemNode *next; // pointer = 4
  int32_t spaceWasterfor16; //4
  int32_t sndspaceWasterfor16; //4
} enemNode_t; // 12 bytes

typedef struct advancedTurretStats {
  int32_t range; //4
  int32_t speed; //4
  int32_t damage; //4
  int32_t type; //4
} advancedTurretStats_t; // 16

typedef struct turretStats {
  advancedTurretStats_t *moreStats; // 4
  pixelPoint_t pixelLocation; // 8
  gridPoint_t gridLocation; //8
} turretStats_t; // 24

typedef struct turret {
  turretStats_t *stats; //4
  int32_t cooldown; // last moment in time to shoot //4
  int32_t uniqueID; //4
} turret_t; //16

typedef struct turrNode{
  turret_t *turret; //4
  struct turrNode *next; //4
  int32_t spaceWasterfor16; //4
  int32_t sndspaceWasterfor16; //4
} turrNode_t; //16

typedef struct tile{
   int32_t state;
   turret_t *turret;
   int32_t spriteLocation;
}tile_t;

typedef struct projectile_s {
  pixelPoint_t curPos; // 8
  pixelPoint_t endPos; // 8
  int32_t active; // 4
  int32_t type; // 4
  float directionX; // 4
  float directionY; // 4
} projectile_t;

typedef struct bloodanim_s {
  int32_t posX; // 4
  int32_t posY; // 4
  imageInfo_t info; // 16
  int32_t active;// 4
  int32_t padding; // 4
} bloodanim_t;

// Enemy functions
extern void enemy_move(enemy_t *enemy, float timeDelta);
extern int32_t enemy_dead(enemy_t *enemy);
extern int32_t enemy_hit(enemy_t *enemy, int32_t damage);
extern pixelPoint_t convertGridtoGridPixelCenter(gridPoint_t gridLocation);
extern void spawnEnemy(void);
extern int32_t waveSizeCalc(int32_t);


// TURRET FUNCTIONS
extern int32_t isWithinRange(pixelPoint_t location1, pixelPoint_t location2, int32_t range);
extern void turretAttack(turret_t *attacker_turret);
extern void upgradeTurret(gridPoint_t position, int32_t upgradeType);
extern void buildTurret(gridPoint_t position, int32_t turretType);
extern void moveTurret(gridPoint_t from, gridPoint_t to);
extern void demolishTurret(gridPoint_t pos);
extern int32_t sizeCooldown(int32_t turretSpeedLevel);

//turretData
// NOTE :Tuesday
extern int32_t initRangeArr[MAX_TURR_TYPE]; // range in pixels
extern int32_t initSpeedArr[MAX_TURR_TYPE];
extern int32_t initDamageArr[MAX_TURR_TYPE];


//map files
//loaded arrays = MAP.file
extern int32_t loadedMap[MAX_LEVELS][MAX_TILES][MAX_TILES];//initial map (and sprite type)
//1,2,3 for given option
extern gridPoint_t loadedEnemyRoute[MAX_LEVELS][ENEMY_ROUTES];
//spawn point


// mouse logic externs
extern void onClick();
extern int32_t isMenuButton(pixelPoint_t pos);
extern int32_t isOption1(pixelPoint_t pos);
extern int32_t isOption2(pixelPoint_t pos);
extern int32_t isOption3(pixelPoint_t pos);
extern int32_t isMove(pixelPoint_t pos);
extern int32_t isDelete(pixelPoint_t pos);
extern int32_t isCancel(pixelPoint_t pos);
extern int32_t isPlayButton(pixelPoint_t pos);
extern int32_t isExitButton(pixelPoint_t pos);
extern int32_t isLevel1(pixelPoint_t pos);
extern int32_t isLevel2(pixelPoint_t pos);
extern int32_t isLevel3(pixelPoint_t pos);
extern int32_t isBack(pixelPoint_t pos);
extern void printUpgrades(void);


// global game data
extern void initWorldData();
extern int32_t selectedLevel;
extern int32_t gameStatus;
extern pixelPoint_t currentPosition;//mouse position
extern int32_t menuPosition;
extern int32_t gold;
extern int32_t lifePoints;
extern int32_t numberOfTurrets;
extern gridPoint_t enemyRoute[ENEMY_ROUTES];
extern tile_t mapGrid[MAX_TILES][MAX_TILES];  // mapgrid[x][y]
extern int32_t oldTime;
extern int32_t prevSpawn;
extern pixelPoint_t mousePos;
extern int32_t spawnedCounter;
extern int32_t waveSize;
extern int32_t allTimeWaves;

//enemyList
extern int32_t getEnemyListLength(void);
extern enemNode_t *addEnem(void);
extern void removeEnem(enemy_t *enemy);
extern enemNode_t *enemHead;


//turretList
extern int32_t getTurretListLength(void);
extern turrNode_t *addTurr(void);
extern void removeTurr(turret_t *turret);
extern turrNode_t *turrHead;

extern projectile_t g_sProjectiles[MAX_PROJECTILES];

inline float inSeconds(int32_t time) {
  return time/(float)SEC_CONST;
}

extern bloodanim_t g_sBloodAnims[MAX_BLOOD_ANIMS];

#endif
