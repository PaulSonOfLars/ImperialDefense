#include "imperial_defense.h"

/*
#define T_GRASS           +0
#define T_ROAD_HORIZON    +1
#define T_ROAD_VERT       +2
#define T_ROAD_TOP_LEFT   +3
#define T_ROAD_TOP_RIGHT  +4
#define T_ROAD_BOT_LEFT   +5
#define T_ROAD_BOT_RIGHT  +6
#define T_GRASS_ROCKS     +7
#define castles  8,9,10
*/
//loaded arrays = MAP.file
//0,1,2 for given option
int32_t loadedMap[MAX_LEVELS][MAX_TILES][MAX_TILES] = {
  //VANILLA MAP
{
{0, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 4, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0},
{7, 0, 7, 7, 2, 0, 0, 6, 1, 1, 5, 0},
{7, 7, 7, 0, 2, 0, 0, 2, 0, 0, 2, 0},
{0, 6, 1, 1, 3, 0, 0, 2, 0, 0, 2, 0},
{0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0},
{0, 2, 0, 7, 7, 7, 0, 2, 0, 0, 2, 0},
{0, 4, 5, 0, 0, 0, 0, 2, 0, 0, 2, 0},
{0, 0, 4, 1, 1, 1, 1, 3, 0, 0, 2, 0},
{7, 0, 7, 0, 0, 0, 0, 0, 0, 0, 2, 0},
{7, 7, 7, 0, 0, 0, 0, 0, 7, 8, 9, 10},
},//initial map (and sprite type)
//SPIRAL MAP
{
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5},
{7, 7, 7, 0, 7, 0, 7, 0, 7, 0, 0, 2},
{7, 7, 0, 7, 0, 7, 0, 7, 0, 0, 0, 2},
{0, 6, 1, 1, 1, 1, 1, 1, 1, 5, 0, 2},
{0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 7, 2},
{0, 2, 0, 0, 8, 9,10, 0, 0, 2, 7, 2},
{0, 2, 0, 0, 8, 9,10, 0, 0, 2, 7, 2},
{0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 7, 2},
{0, 2, 7, 0, 0, 2, 0, 0, 0, 2, 7, 2},
{0, 2, 0, 0, 0, 4, 1, 1, 1, 3, 0, 2},
{0, 2, 0, 0, 7, 0, 0, 0, 0, 0, 0, 2},
{7, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3},
},
//WATER MAP
{
{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
{0, 0, 0, 0, 8, 9,10, 7, 7, 7, 7, 7},
{0, 0, 0, 0, 8, 9,10, 7, 7, 7, 7, 7},
{0, 0, 0, 0, 0, 2, 0, 7, 7, 7, 7, 7},
{0, 0, 0, 0, 0, 2, 0, 7, 7, 7, 7, 7},
{0, 0, 0, 0, 0, 2, 0, 7, 7, 7, 7, 0},
{1, 1, 1, 1, 1, 3, 0, 7, 7, 7, 7, 7},
{0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7},
{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7},
},
};
// corners of the route, and marked as grid locatoins, not as pixels

gridPoint_t loadedEnemyRoute[MAX_LEVELS][ENEMY_ROUTES] = {
//VANILLA map route
{
{0,11},{0,1},{2,1},{2,4},{5,4},{5,1},{8,1},
{8,2},{9,2},{9,7},{3,7},{3,10},{10,10}
},
//spiral map route
{
{0,0},{0,11},{11,11},{11,1},{3,1},{3,9},{9,9},{9,5},{7,5},
//hacked
{7,5},{7,5},{7,5},{7,5}
},
//water map route
{
{8,0},{8,5},{5,5}
},
};

// global stats
int32_t selectedLevel;
pixelPoint_t currentPosition;//joistick position //NOTE ????
int32_t menuPosition;
int32_t numberOfTurrets;
int32_t gold;
int32_t lifePoints;
gridPoint_t enemyRoute[ENEMY_ROUTES];
tile_t mapGrid[MAX_TILES][MAX_TILES];  // mapgrid[x][y]
int32_t gameStatus;


void initWorldData(void);
void initTurretStruct(void);//to avoid using memset(void)
void initEnemyRoute(void);
void squareMapInit(void);
void initEnemyStruct(void);
int32_t generateSprite(int32_t);

//-------------------IMPLEMENTATIONS-------------------

void initWorldData(void) {
  // set highlighted menu option to top one
    menuPosition = TOP_MENU_BAR;
    //inits basic values
    numberOfTurrets = 0;
    gold = START_GOLD;
    lifePoints = START_LP;
    squareMapInit(); //sets the 12x12 grid to empty
    initEnemyRoute(); // loads enem route
    oldTime = 0;
    prevSpawn = 0;
    spawnedCounter = 0;
    waveSize = 0;
    // NOTE: Tuesday
    allTimeWaves = 0;
    return;
}

void initEnemyRoute(void) {
  for(int32_t i = 0; i < ENEMY_ROUTES; i++) {
    enemyRoute[i].x = loadedEnemyRoute[selectedLevel][i].x;
    enemyRoute[i].y = loadedEnemyRoute[selectedLevel][i].y;
  }
}

int32_t setTileState(int32_t val){
  switch(val) {
    case 0: // grass
      return BUILD_ZONE;
      break;

    case 7: // rocks
    case 8: // castles
    case 9:
    case 10:
      return UNUSABLE_ZONE;

    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6: // roads
      return WALK_ZONE;
      break;
    default:
      return UNUSABLE_ZONE; // default
  }
}

void squareMapInit(void) {

  for(int32_t i = 0; i < MAX_TILES; i++)
    for(int32_t j = 0; j < MAX_TILES; j++) {
      mapGrid[i][j].state = setTileState(loadedMap[selectedLevel][i][j]);
      mapGrid[i][j].turret = NULL;

      mapGrid[i][j].spriteLocation = generateSprite(loadedMap[selectedLevel][i][j]);
    }
}

int32_t generateSprite(int32_t type) {
  // 0 BUILD_ZONE , 2 - ROAD , 4 - ROCk 5,6,7 -castle - else grass
  if(type <= 7 && type >= 0) {
    // should return grass/road  if defines kept at offset IN_GAME_TEXTURES
    return IN_GAME_TEXTURES + type;
  }// same as grass/road but with castle positions
  else if( type >= 8 &&  type <=10){
    return T_CASTLE_1 + type - 8;
  }// if incorect data give it grass rock(unusable)
  else return T_GRASS_ROCKS;
}
