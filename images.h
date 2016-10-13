#ifndef __IMAGES_H__
#define __IMAGES_H__

#include <stdint.h>

#define MAX_ANIMATION_FRAMES 4

#define IMAGE_LOADING       0
#define IMAGE_LEVELPICK     1
#define IMAGE_TEXT_DEFAULT  2
#define IMAGE_TEXT_BUILD    3
#define IMAGE_TEXT_UPGRADE  4
#define IMAGE_INGAME_MENU   5
#define IMAGE_GAME_START    6
#define IMAGE_CURSOR        7
#define IMAGE_CREDITS       8

#define IMAGE_MAP_ONE        10
#define IMAGE_MAP_TWO        11
#define IMAGE_MAP_THREE      12

//TEXTURE POSITIONS
#define IN_GAME_TEXTURES    13
/*
//if you want later to add water
//cuz we decided that unbuildable terrain will be just rocks to easy
//match tiles textures
// if you add water you have to edit the game Init texture part
// so that it picks proper(visually) sprite for each tiles

#define T_WATER           IN_GAME_TEXTURES+7
#define T_WATER_TOP       IN_GAME_TEXTURES+8
#define T_WATER_BOTTOM    IN_GAME_TEXTURES+9
#define T_WATER_LEFT      IN_GAME_TEXTURES+10
#define T_WATER_RIGHT     IN_GAME_TEXTURES+11
#define T_WATER_TOP_LEFT  IN_GAME_TEXTURES+12
#define T_WATER_TOP_RIGHT IN_GAME_TEXTURES+13
#define T_WATER_BOT_LEFT  IN_GAME_TEXTURES+14
#define T_WATER_BOT_RIGHT IN_GAME_TEXTURES+15
#define T_WATER_ALL       IN_GAME_TEXTURES+16
*/

//TODO: remove redundant _1 _2 cuz we use offset of the type
//NOTE: KEEP those to OFFest 0-7 of IN_GAME_TEXTURES in order for quick
//      texture generator for the map to work
#define T_GRASS           IN_GAME_TEXTURES+0
#define T_ROAD_X          IN_GAME_TEXTURES+1
#define T_ROAD_Y          IN_GAME_TEXTURES+2
#define T_ROAD_TOP_LEFT   IN_GAME_TEXTURES+3
#define T_ROAD_TOP_RIGHT  IN_GAME_TEXTURES+4
#define T_ROAD_BOT_LEFT   IN_GAME_TEXTURES+5
#define T_ROAD_BOT_RIGHT  IN_GAME_TEXTURES+6
#define T_GRASS_ROCKS     IN_GAME_TEXTURES+7

#define T_ENEMY_1         IN_GAME_TEXTURES+8
#define T_ENEMY_2         T_ENEMY_1+1

#define T_TURRET_1        IN_GAME_TEXTURES+10
#define T_TURRET_2        T_TURRET_1+1
#define T_TURRET_3        T_TURRET_1+2

#define T_CASTLE_1        IN_GAME_TEXTURES+13
#define T_CASTLE_2        T_CASTLE_1+1
#define T_CASTLE_3        T_CASTLE_1+2

#define PRETTY_BUTTONS     T_CASTLE_3+1
#define PRETTY_PLAY       PRETTY_BUTTONS+1
#define PRETTY_EXIT       PRETTY_BUTTONS+2
#define PRETTY_MENU       PRETTY_BUTTONS+3
#define PRETTY_ARCHER     PRETTY_BUTTONS+4
#define PRETTY_CANON      PRETTY_BUTTONS+5
#define PRETTY_MAGE       PRETTY_BUTTONS+6
#define PRETTY_RANGE      PRETTY_BUTTONS+7
#define PRETTY_SPEED      PRETTY_BUTTONS+8
#define PRETTY_DAMAGE     PRETTY_BUTTONS+9
#define PRETTY_MOV        PRETTY_BUTTONS+10
#define PRETTY_DEL        PRETTY_BUTTONS+11
#define PRETTY_CANCEL     PRETTY_BUTTONS+12
#define PRETTY_L1         PRETTY_BUTTONS+13
#define PRETTY_L2         PRETTY_BUTTONS+14
#define PRETTY_L3         PRETTY_BUTTONS+15
#define PRETTY_BACK       PRETTY_BUTTONS+16

#define IMAGE_FONT        PRETTY_BACK+1

#define PROJ_ARROW        IMAGE_FONT+1
#define PROJ_CANNON       PROJ_ARROW+1
#define PROJ_MAGIC        PROJ_ARROW+2

#define IMAGE_BLOOD       PROJ_MAGIC+1

#define IMAGE_HOVER_ARCHER  IMAGE_BLOOD+1
#define IMAGE_HOVER_CANON   IMAGE_HOVER_ARCHER+1
#define IMAGE_HOVER_MAGE    IMAGE_HOVER_ARCHER+2

#define IMAGE_TILE_SEL  IMAGE_HOVER_MAGE+1

#define IMAGE_NUM         IMAGE_TILE_SEL+1 // keep this last!!! always one bigger then last image

//top left is origin
// ------MAIN MENU BUTTONS-------
#define START_BUTTON_X 512
#define START_BUTTON_Y 390
#define EXIT_BUTTON_X 512
#define EXIT_BUTTON_Y 510

#define MAIN_MENU_BUTTON_WIDTH 244
#define MAIN_MENU_BUTTON_HEIGHT 80

// ------IN GAME BUTTONS---------
#define MENU_BUTTON_X 896
#define MENU_BUTTON_Y 56

#define TIME_COUNTER_X 936
#define TIME_COUNTER_Y 146

#define FIRST_BUTTON_X 896
#define FIRST_BUTTON_Y 342
#define SECOND_BUTTON_X 896
#define SECOND_BUTTON_Y 432
#define THIRD_BUTTON_X 896
#define THIRD_BUTTON_Y 522

#define BUTTON_WIDTH 244
#define BUTTON_HEIGHT 80

#define SMALL_BUTTON_WIDTH 128
#define SMALL_BUTTON_HEIGHT 80

#define MOVE_X 832
#define MOVE_Y 626
#define DELETE_X 960
#define DELETE_Y 626

#define HEALTH_TEXT_X 956
#define HEALTH_TEXT_Y 234
#define GOLD_TEXT_X 836
#define GOLD_TEXT_Y 234

#define CANCEL_X 896
#define CANCEL_Y 716

#define UPGRADE 1
#define BUILD 2

#define BUTTON_L1_X 300
#define BUTTON_L1_Y 436

#define BUTTON_L2_X 512
#define BUTTON_L2_Y 436

#define BUTTON_L3_X 718
#define BUTTON_L3_Y 436

#define BUTTON_BACK_X 510
#define BUTTON_BACK_Y 630

#define BUTTON_LEVEL_SIZE 160

typedef struct imageInfo_s {
  int32_t width; // 4
  int32_t height; // 4
  int32_t widthReal; // 4
  int32_t currentFrame; // 4
  float timeElapsed; // 4
  int32_t padding[3]; // 12
} imageInfo_t;

typedef struct images{
  int32_t *pBinaryAddress;
  int32_t animated;
  int32_t maxFrames;
  int32_t animationFPS;//in milisecond
  float timePerFrame;
  imageInfo_t info;
} image_t;

// functions
extern void init_images();

extern image_t *totalSetOfImages[IMAGE_NUM];
extern int32_t sidePaneText; // 1 = default, 2 = build, 3 = upgrade
extern int32_t gamePaneDisplay;


#endif
