#include "backbone.h"
#include "images.h"
#include "imperial_defense.h"

image_t *totalSetOfImages[IMAGE_NUM];


image_t image_loading;
extern int32_t *bin_img_loading;

image_t image_levelpick;
extern int32_t *bin_img_levelpick;

image_t image_cursor;
extern int32_t *bin_img_cursor;

image_t image_credits;
extern int32_t *bin_img_credits;

image_t image_font;
extern int32_t *bin_img_font;

// ----------- GAME PANES ---------------

image_t image_text_default;
image_t image_text_build;
image_t image_text_upgrade;

image_t image_pretty_play;
image_t image_pretty_exit;
image_t image_pretty_menu;
image_t image_pretty_archer;
image_t image_pretty_canon;
image_t image_pretty_mage;
image_t image_pretty_range;
image_t image_pretty_dmg;
image_t image_pretty_speed;
image_t image_pretty_mov;
image_t image_pretty_del;
image_t image_pretty_cancel;

image_t image_hover_archer;
image_t image_hover_canon;
image_t image_hover_mage;


extern int32_t *bin_img_view_select_default;
extern int32_t *bin_img_view_select_tower;
extern int32_t *bin_img_view_select_upgrade;

extern int32_t *bin_img_pretty_play;
extern int32_t *bin_img_pretty_exit;
extern int32_t *bin_img_pretty_menu;
extern int32_t *bin_img_pretty_archer;
extern int32_t *bin_img_pretty_canon;
extern int32_t *bin_img_pretty_mage;
extern int32_t *bin_img_pretty_speed;
extern int32_t *bin_img_pretty_range;
extern int32_t *bin_img_pretty_dmg;
extern int32_t *bin_img_pretty_mov;
extern int32_t *bin_img_pretty_del;
extern int32_t *bin_img_pretty_cancel;

extern int32_t *bin_img_archer_hover;
extern int32_t *bin_img_canon_hover;
extern int32_t *bin_img_mage_hover;

// ----------- GAME TILE ---------------
image_t image_map_one;
image_t image_map_two;
image_t image_map_three;

//NOTE: not required as full map loaded
// image_t image_cornerDL;
// image_t image_cornerUL;
// image_t image_cornerDR;
// image_t image_cornerUR;
//
// image_t image_pathHor;
// image_t image_pathVer;
// image_t image_grass;
// // image_t image_grassflowers;
// // image_t image_grasswood;
// image_t image_bricks;

extern int32_t *bin_img_map_one;
extern int32_t *bin_img_map_two;
extern int32_t *bin_img_map_three;

//NOTE: not required as full map loaded
// extern int32_t *bin_img_cornerDL;
// extern int32_t *bin_img_cornerUL;
// extern int32_t *bin_img_cornerDR;
// extern int32_t *bin_img_cornerUR;
//
// extern int32_t *bin_img_pathHor;
// extern int32_t *bin_img_pathVer;
// extern int32_t *bin_img_grass;
// extern int32_t *bin_img_grassflowers;
// extern int32_t *bin_img_grasswood;
// extern int32_t *bin_img_bricks;

// ----------- ENEMIES ----------------

// TODO: how to load animated pictures

image_t image_barbar;
image_t image_boar;

extern int32_t *bin_img_barbar;
extern int32_t *bin_img_boar;

//------------ TOWERS ----------------

image_t image_archer;
image_t image_cannon;
image_t image_mage;

extern int32_t *bin_img_archer;
extern int32_t *bin_img_cannon;
extern int32_t *bin_img_mage;

//------------ PROJECTILES ----------------
image_t image_projArrow;
image_t image_projCannon;
image_t image_projMagic;

extern int32_t *bin_img_projArrow;
extern int32_t *bin_img_projCannon;
extern int32_t *bin_img_projMagic;

image_t image_blood;
extern int32_t *bin_img_blood;

image_t image_level_1h;
extern int32_t *bin_img_level_1h;

image_t image_level_2h;
extern int32_t *bin_img_level_2h;

image_t image_level_3h;
extern int32_t *bin_img_level_3h;

image_t image_back_h;
extern int32_t *bin_img_back_h;

image_t image_tile_sel;
extern int32_t *bin_img_tile_sel;


//NOTE: what am i doing here
// we don't know Paul
// but there is a good job at McDonalds I heard
int32_t sidePaneText = IMAGE_TEXT_DEFAULT;
int32_t gamePaneText = IMAGE_GAME_START;

void initializeImage(image_t *img, int32_t *pBinaryAddress, int isAnimated,
  int maxFrame, int animFPS) {

  // extract size from the binary file
  img->info.width = *pBinaryAddress;
  img->info.height = *(pBinaryAddress+1);
  img->info.currentFrame = 0;
  img->info.widthReal = img->info.width;
  img->animated = isAnimated;
  img->maxFrames = maxFrame;
  img->animationFPS = animFPS;
  img->pBinaryAddress = pBinaryAddress+8; // skip first two integers
  if(isAnimated) {
    img->info.currentFrame = 0;
    img->info.widthReal /= (float)maxFrame;
  }
  img->timePerFrame = 1.0f/(float)img->animationFPS;
  img->info.timeElapsed = 0.0f;
}


// in this function, initialize the image you want to use with correct arguments
// and then put it into totalSetOfImages array which hold all the images which
// are used in the game. (!) DO NOT OVERWRITE OTHER IMAGES IN THE ARRAY
void init_images() {
  for(int i = 0; i < IMAGE_NUM; i++)
    totalSetOfImages[i] = 0;

  initializeImage(&image_loading, (int32_t*)&bin_img_loading, 0, 1, 0);
  initializeImage(&image_levelpick, (int32_t*)&bin_img_levelpick,0,1,0);
  initializeImage(&image_cursor, (int32_t*)&bin_img_cursor, 0, 1, 0);
  initializeImage(&image_credits, (int32_t*)&bin_img_credits, 0, 1, 0);
  initializeImage(&image_text_default, (int32_t*)&bin_img_view_select_default, 0, 1, 0);
  initializeImage(&image_text_build, (int32_t*)&bin_img_view_select_tower, 0, 1, 0);
  initializeImage(&image_text_upgrade, (int32_t*)&bin_img_view_select_upgrade, 0, 1, 0);

  initializeImage(&image_pretty_play, (int32_t*)&bin_img_pretty_play, 0,1,0);
  initializeImage(&image_pretty_exit, (int32_t*)&bin_img_pretty_exit, 0,1,0);
  initializeImage(&image_pretty_menu, (int32_t*)&bin_img_pretty_menu, 0,1,0);
  initializeImage(&image_pretty_archer, (int32_t*)&bin_img_pretty_archer, 0,1,0);
  initializeImage(&image_pretty_canon, (int32_t*)&bin_img_pretty_canon, 0,1,0);
  initializeImage(&image_pretty_mage, (int32_t*)&bin_img_pretty_mage, 0,1,0);
  initializeImage(&image_pretty_range, (int32_t*)&bin_img_pretty_range, 0,1,0);
  initializeImage(&image_pretty_speed, (int32_t*)&bin_img_pretty_speed, 0,1,0);
  initializeImage(&image_pretty_dmg, (int32_t*)&bin_img_pretty_dmg, 0,1,0);
  initializeImage(&image_pretty_mov, (int32_t*)&bin_img_pretty_mov, 0,1,0);
  initializeImage(&image_pretty_del, (int32_t*)&bin_img_pretty_del, 0,1,0);
  initializeImage(&image_pretty_cancel, (int32_t*)&bin_img_pretty_cancel, 0,1,0);
  initializeImage(&image_level_1h, (int32_t*)&bin_img_level_1h, 0,1,0);
  initializeImage(&image_level_2h, (int32_t*)&bin_img_level_2h, 0,1,0);
  initializeImage(&image_level_3h, (int32_t*)&bin_img_level_3h, 0,1,0);
  initializeImage(&image_back_h, (int32_t*)&bin_img_back_h, 0,1,0);

  initializeImage(&image_hover_archer,(int32_t*)&bin_img_archer_hover,0,1,0);
  initializeImage(&image_hover_canon,(int32_t*)&bin_img_canon_hover,0,1,0);
  initializeImage(&image_hover_mage,(int32_t*)&bin_img_mage_hover,0,1,0);


  initializeImage(&image_map_one, (int32_t*)&bin_img_map_one,0,1,0);
  initializeImage(&image_map_two, (int32_t*)&bin_img_map_two,0,1,0);
  initializeImage(&image_map_three, (int32_t*)&bin_img_map_three,0,1,0);

  //NOTE: not required as full map loaded

  // initializeImage(&image_cornerDL, (int32_t*)&bin_img_cornerDL, 0, 1, 0);
  // initializeImage(&image_cornerUL, (int32_t*)&bin_img_cornerUL, 0, 1, 0);
  // initializeImage(&image_cornerDR, (int32_t*)&bin_img_cornerDR, 0, 1, 0);
  // initializeImage(&image_cornerUR, (int32_t*)&bin_img_cornerUR, 0, 1, 0);
  //
  // initializeImage(&image_pathHor, (int32_t*)&bin_img_pathHor, 0, 1, 0);
  // initializeImage(&image_pathVer, (int32_t*)&bin_img_pathVer, 0, 1, 0);
  //
  // initializeImage(&image_grass, (int32_t*)&bin_img_grass, 0, 1, 0);
  // // initializeImage(&image_grasswood, (int32_t*)&bin_img_grasswood, 0, 1, 0);
  // // initializeImage(&image_grassflowers, (int32_t*)&bin_img_grassflowers, 0, 1, 0);
  // initializeImage(&image_bricks, (int32_t*)&bin_img_bricks, 0, 1, 0);


  initializeImage(&image_barbar, (int32_t*)&bin_img_barbar, 1, 3, 10);
  initializeImage(&image_boar, (int32_t*)&bin_img_boar, 1, 4, 10);

  initializeImage(&image_archer, (int32_t*)&bin_img_archer, 0, 1, 0);
  initializeImage(&image_cannon, (int32_t*)&bin_img_cannon, 0, 1, 0);
  initializeImage(&image_mage, (int32_t*)&bin_img_mage, 0, 1, 0);

  // set up font image
  // its not animated but I just use the existing offset behaviour
  // font image contains ASCII table in a single row
  // so there are 256 positions to go to
  initializeImage(&image_font, (int32_t*)&bin_img_font, 0, 256, 0);
  image_font.info.widthReal /= (float)256;

  initializeImage(&image_projArrow, (int32_t*)&bin_img_projArrow, 0, 1, 0);
  initializeImage(&image_projCannon, (int32_t*)&bin_img_projCannon, 0, 1, 0);
  initializeImage(&image_projMagic, (int32_t*)&bin_img_projMagic, 0, 1, 0);

  initializeImage(&image_blood, (int32_t*)&bin_img_blood, 1, 6, 20);

  initializeImage(&image_tile_sel, (int32_t*)&bin_img_tile_sel,0,1,0);

  // set up the pointers
  totalSetOfImages[IMAGE_LOADING] = &image_loading;
  totalSetOfImages[IMAGE_LEVELPICK] = &image_levelpick;
  totalSetOfImages[IMAGE_CURSOR] = &image_cursor;
  totalSetOfImages[IMAGE_CREDITS] = &image_credits;
  totalSetOfImages[IMAGE_TEXT_DEFAULT] = &image_text_default;
  totalSetOfImages[IMAGE_TEXT_BUILD] = &image_text_build;
  totalSetOfImages[IMAGE_TEXT_UPGRADE] = &image_text_upgrade;

  totalSetOfImages[PRETTY_PLAY] = &image_pretty_play;
  totalSetOfImages[PRETTY_EXIT] = &image_pretty_exit;
  totalSetOfImages[PRETTY_MENU] = &image_pretty_menu;
  totalSetOfImages[PRETTY_ARCHER] = &image_pretty_archer;
  totalSetOfImages[PRETTY_CANON] = &image_pretty_canon;
  totalSetOfImages[PRETTY_MAGE] = &image_pretty_mage;
  totalSetOfImages[PRETTY_SPEED] = &image_pretty_speed;
  totalSetOfImages[PRETTY_RANGE] = &image_pretty_range;
  totalSetOfImages[PRETTY_DAMAGE] = &image_pretty_dmg;
  totalSetOfImages[PRETTY_MOV] = &image_pretty_mov;
  totalSetOfImages[PRETTY_DEL] = &image_pretty_del;
  totalSetOfImages[PRETTY_CANCEL] = &image_pretty_cancel;
  totalSetOfImages[PRETTY_L1] = &image_level_1h;
  totalSetOfImages[PRETTY_L2] = &image_level_2h;
  totalSetOfImages[PRETTY_L3] = &image_level_3h;
  totalSetOfImages[PRETTY_BACK] = &image_back_h;

  totalSetOfImages[IMAGE_HOVER_ARCHER] = &image_hover_archer;
  totalSetOfImages[IMAGE_HOVER_CANON] = &image_hover_canon;
  totalSetOfImages[IMAGE_HOVER_MAGE] = &image_hover_mage;

  totalSetOfImages[IMAGE_MAP_ONE] = &image_map_one;
  totalSetOfImages[IMAGE_MAP_TWO] = &image_map_two;
  totalSetOfImages[IMAGE_MAP_THREE] = &image_map_three;

  //NOTE: not required as full map loaded

  // totalSetOfImages[T_ROAD_BOT_LEFT] = &image_cornerDL;
  // totalSetOfImages[T_ROAD_TOP_LEFT] = &image_cornerUL;
  // totalSetOfImages[T_ROAD_BOT_RIGHT] = &image_cornerDR;
  // totalSetOfImages[T_ROAD_TOP_RIGHT] = &image_cornerUR;
  //
  // totalSetOfImages[T_ROAD_X] = &image_pathHor;
  // totalSetOfImages[T_ROAD_Y] = &image_pathVer;
  //
  // totalSetOfImages[T_GRASS] = &image_grass;
  // // totalSetOfImages[] = &image_grasswood // no texture mapping
  // // totalSetOfImages[] = &image_grassflowers // no texture mapping
  // totalSetOfImages[T_GRASS_ROCKS] = &image_bricks;

  totalSetOfImages[T_ENEMY_1] = &image_barbar;
  totalSetOfImages[T_ENEMY_2] = &image_boar;

  totalSetOfImages[T_TURRET_1] = &image_archer;
  totalSetOfImages[T_TURRET_2] = &image_cannon;
  totalSetOfImages[T_TURRET_3] = &image_mage;

  totalSetOfImages[IMAGE_FONT] = &image_font;

  totalSetOfImages[PROJ_ARROW] = &image_projArrow;
  totalSetOfImages[PROJ_CANNON] = &image_projCannon;
  totalSetOfImages[PROJ_MAGIC] = &image_projMagic;

  totalSetOfImages[IMAGE_BLOOD] = &image_blood;
  totalSetOfImages[IMAGE_TILE_SEL] = &image_tile_sel;
}
