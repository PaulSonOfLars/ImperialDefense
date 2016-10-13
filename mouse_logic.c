#include "imperial_defense.h"
#include "backbone.h"

// implement some sort of menu logic;
// start with a click to start screen
// redirects to main menu, with options to start new game, load, or credits
// new game redirects to a map chooser -> just one large image separated into x amount of chunks for each map
// load game redirects to 3 "save" files (that have been pre-compiled as new games) (should add a delete function here)

// starting game then uses ingame mouse logic



int32_t isMoveAction = 0; // T or F
gridPoint_t prevPos;

int32_t isInSidePane(pixelPoint_t pos);
int32_t isInGameGrid(pixelPoint_t pos);
int32_t isMenuButton(pixelPoint_t pos);
int32_t isOption1(pixelPoint_t pos);
int32_t isOption2(pixelPoint_t pos);
int32_t isOption3(pixelPoint_t pos);
int32_t isMove(pixelPoint_t pos);
int32_t isDelete(pixelPoint_t pos);
int32_t isCancel(pixelPoint_t pos);
pixelPoint_t mousePos;
void optionExecute(int32_t option, pixelPoint_t pos);



pixelPoint_t getMousePos(void) {
  //NOTE: part of mouse_driver
  mousePos.x = g_iMousePosX;
  mousePos.y = g_iMousePosY;
  return mousePos;
}

// function to check if two points are equal
// int32_t equals(point_t one, point_t two) {
//   return ((one.x) == (two.x)) && ((one.y) == (two.y));
// }



void optionExecute(int32_t option, pixelPoint_t pos) {
  if (sidePaneText == IMAGE_TEXT_BUILD) {
    buildTurret(prevPos, option);
    sidePaneText = IMAGE_TEXT_DEFAULT;
    isMoveAction = 0;
  } else if (sidePaneText == IMAGE_TEXT_UPGRADE) {
    upgradeTurret(prevPos, option);
    // upgrading should be possible to do multiple times, so pane shouldnt change
    isMoveAction = 0;
  } else {
    // NOTE: can't get here
  }
}



void pressTile(pixelPoint_t pos) {
  prevPos.y = ((int32_t)((float)(pos.x / TILE_PIXEL_SIZE))); // grid pos
  prevPos.x = ((int32_t)((float)(pos.y / TILE_PIXEL_SIZE)));


  tile_t tile = mapGrid[prevPos.x][prevPos.y];
  if (tile.state == TURRET_OCCUPIED) {
    sidePaneText = IMAGE_TEXT_UPGRADE;
  } else if (tile.state == BUILD_ZONE){
    sidePaneText = IMAGE_TEXT_BUILD;
  } else {
    sidePaneText = IMAGE_TEXT_DEFAULT;
  }
  //NOTE: else UNUSABLE_ZONE or WALK_ZONE and turrets cant be built
}


void pressButtons(pixelPoint_t pos) {
  if (isOption1(pos)) {
    optionExecute(FIRST_OPTION, pos);

  } else if (isOption2(pos)) {
    optionExecute(SECOND_OPTION, pos);

  } else if (isOption3(pos)) {
    optionExecute(THIRD_OPTION, pos);

  } else if (isMove(pos)) {
    isMoveAction = 1;

  } else if (isDelete(pos)) {
    demolishTurret(prevPos);

  } else if (isCancel(pos)) {
    sidePaneText = IMAGE_TEXT_DEFAULT;
    isMoveAction = 0; // sets to false
  }
}



void onClick(void) {
  pixelPoint_t pos = getMousePos();

  if (gameStatus == GAME_INGAME) {
    if (isMenuButton(pos)) {
      gameStatus = GAME_INMENU;
      //resets data
      while (turrHead != NULL) {
        removeTurr(turrHead->turret);
      }
      while (enemHead != NULL) {
        removeEnem(enemHead->enemy);
      }
    // NOTE: should reset
    } else if (isInGameGrid(pos)) {
     if (isMoveAction) {
       gridPoint_t translatedPos;
       translatedPos.y = ((int32_t)((float)(pos.x / TILE_PIXEL_SIZE)));
       translatedPos.x = ((int32_t)((float)(pos.y / TILE_PIXEL_SIZE)));
       moveTurret(prevPos, translatedPos);
       isMoveAction = 0;
       sidePaneText = IMAGE_TEXT_DEFAULT;
       return;
     } else {
       pressTile(pos);
     }
    } else if (isInSidePane(pos)) {
        if (sidePaneText != IMAGE_TEXT_DEFAULT) {
          pressButtons(pos);
        } else {
          // ignore
        }
    }
    // else do nothing
  } else if(gameStatus == GAME_INMENU){
    if (isPlayButton(pos)) { // click play
      gameStatus = GAME_LEVELPICK;
    } else if (isExitButton(pos)) { // click exit button
      //TODO: some Kill Kernel function?
      gameStatus = GAME_CREDITS;
    }
  } else if(gameStatus == GAME_LEVELPICK) {
    if(isLevel1(pos)) {
      selectedLevel = 0;
      gameStatus = GAME_INGAME;
      sidePaneText = IMAGE_TEXT_DEFAULT;
      initWorldData();
    } else if(isLevel2(pos)) {
      selectedLevel = 1;
      gameStatus = GAME_INGAME;
      sidePaneText = IMAGE_TEXT_DEFAULT;
      initWorldData();
    } else if(isLevel3(pos)) {
      selectedLevel = 2;
      gameStatus = GAME_INGAME;
      sidePaneText = IMAGE_TEXT_DEFAULT;
      initWorldData();
    } else if(isBack(pos)) {
      gameStatus = GAME_INMENU;
      return;
    }
  }
}

void printUpgrades(void) {
  // NOTE: Tuesday
  advancedTurretStats_t *stats = mapGrid[prevPos.x][prevPos.y].turret->stats->moreStats;
  DrawText(itoa((stats->speed - initSpeedArr[stats->type]+UPGRADE_SPEED_AMOUNT) * UPGRADE_COST, 10), HEALTH_TEXT_X - 20, FIRST_BUTTON_Y);
  DrawText(itoa((stats->range - initRangeArr[stats->type]+UPGRADE_RANGE_AMOUNT) / PIXELS_PER_UPGRADE * UPGRADE_COST, 10), HEALTH_TEXT_X - 20, SECOND_BUTTON_Y);
  DrawText(itoa((stats->damage -initDamageArr[stats->type]+UPGRADE_DAMAGE_AMOUNT) * UPGRADE_COST, 10), HEALTH_TEXT_X - 20, THIRD_BUTTON_Y);
  DrawImageTr(IMAGE_TILE_SEL,  mapGrid[prevPos.x][prevPos.y].turret->stats->pixelLocation.x,
   mapGrid[prevPos.x][prevPos.y].turret->stats->pixelLocation.y);
}

//------LOCATION LOGIC-----------
int32_t isInSidePane(pixelPoint_t pos) {
  return (pos.x < SIDE_PANEL_X + SIDE_PANEL_WIDTH/2)
      && (pos.y < SIDE_PANEL_Y + SIDE_PANEL_HEIGHT/2)
      && (pos.x > SIDE_PANEL_X - SIDE_PANEL_WIDTH/2)
      && (pos.y > SIDE_PANEL_Y - SIDE_PANEL_HEIGHT/2);
}



int32_t isInGameGrid(pixelPoint_t pos) {
  return (pos.x < GAME_PANEL_X + GAME_PANEL_WIDTH/2)
      && (pos.y < GAME_PANEL_Y + GAME_PANEL_HEIGHT/2)
      && (pos.x > GAME_PANEL_X - GAME_PANEL_WIDTH/2)
      && (pos.y > GAME_PANEL_Y - GAME_PANEL_HEIGHT/2);
}

int32_t isPlayButton(pixelPoint_t pos) {
  return (pos.x < START_BUTTON_X + MAIN_MENU_BUTTON_WIDTH/2)
      && (pos.y < START_BUTTON_Y + MAIN_MENU_BUTTON_HEIGHT/2)
      && (pos.x > START_BUTTON_X - MAIN_MENU_BUTTON_WIDTH/2)
      && (pos.y > START_BUTTON_Y - MAIN_MENU_BUTTON_HEIGHT/2);
}

int32_t isExitButton(pixelPoint_t pos) {
  return (pos.x < EXIT_BUTTON_X + MAIN_MENU_BUTTON_WIDTH/2)
      && (pos.y < EXIT_BUTTON_Y + MAIN_MENU_BUTTON_HEIGHT/2)
      && (pos.x > EXIT_BUTTON_X - MAIN_MENU_BUTTON_WIDTH/2)
      && (pos.y > EXIT_BUTTON_Y - MAIN_MENU_BUTTON_HEIGHT/2);
}

int32_t isMenuButton(pixelPoint_t pos) {
  return (pos.x < MENU_BUTTON_X + BUTTON_WIDTH/2)
      && (pos.y < MENU_BUTTON_Y + BUTTON_HEIGHT/2)
      && (pos.x > MENU_BUTTON_X - BUTTON_WIDTH/2)
      && (pos.y > MENU_BUTTON_Y - BUTTON_HEIGHT/2);
}

int32_t isOption1(pixelPoint_t pos) {
  return (pos.x < FIRST_BUTTON_X + BUTTON_WIDTH/2)
      && (pos.y < FIRST_BUTTON_Y + BUTTON_HEIGHT/2)
      && (pos.x > FIRST_BUTTON_X - BUTTON_WIDTH/2)
      && (pos.y > FIRST_BUTTON_Y - BUTTON_HEIGHT/2);
}

int32_t isOption2(pixelPoint_t pos) {
  return (pos.x < SECOND_BUTTON_X + BUTTON_WIDTH/2)
      && (pos.y < SECOND_BUTTON_Y + BUTTON_HEIGHT/2)
      && (pos.x > SECOND_BUTTON_X - BUTTON_WIDTH/2)
      && (pos.y > SECOND_BUTTON_Y - BUTTON_HEIGHT/2);
}

int32_t isOption3(pixelPoint_t pos) {
  return (pos.x < THIRD_BUTTON_X + BUTTON_WIDTH/2)
      && (pos.y < THIRD_BUTTON_Y + BUTTON_HEIGHT/2)
      && (pos.x > THIRD_BUTTON_X - BUTTON_WIDTH/2)
      && (pos.y > THIRD_BUTTON_Y - BUTTON_HEIGHT/2);
}

int32_t isMove(pixelPoint_t pos) {
  return (sidePaneText == IMAGE_TEXT_UPGRADE)
      && (pos.x < MOVE_X + SMALL_BUTTON_WIDTH/2)
      && (pos.y < MOVE_Y + SMALL_BUTTON_HEIGHT/2)
      && (pos.x > MOVE_X - SMALL_BUTTON_WIDTH/2)
      && (pos.y > MOVE_Y - SMALL_BUTTON_HEIGHT/2);
}

int32_t isDelete(pixelPoint_t pos) {
  return (sidePaneText == IMAGE_TEXT_UPGRADE)
      && (pos.x < DELETE_X + SMALL_BUTTON_WIDTH/2)
      && (pos.y < DELETE_Y + SMALL_BUTTON_HEIGHT/2)
      && (pos.x > DELETE_X - SMALL_BUTTON_WIDTH/2)
      && (pos.y > DELETE_Y - SMALL_BUTTON_HEIGHT/2);
}

int32_t isCancel(pixelPoint_t pos) {
  return (pos.x < CANCEL_X + BUTTON_WIDTH/2)
      && (pos.y < CANCEL_Y + BUTTON_HEIGHT/2)
      && (pos.x > CANCEL_X - BUTTON_WIDTH/2)
      && (pos.y > CANCEL_Y - BUTTON_HEIGHT/2);
}

int32_t isLevel1(pixelPoint_t pos) {
  return (pos.x < BUTTON_L1_X + BUTTON_LEVEL_SIZE/2)
      && (pos.y < BUTTON_L1_Y + BUTTON_LEVEL_SIZE/2)
      && (pos.x > BUTTON_L1_X - BUTTON_LEVEL_SIZE/2)
      && (pos.y > BUTTON_L1_Y - BUTTON_LEVEL_SIZE/2);
}

int32_t isLevel2(pixelPoint_t pos) {
  return (pos.x < BUTTON_L2_X + BUTTON_LEVEL_SIZE/2)
      && (pos.y < BUTTON_L2_Y + BUTTON_LEVEL_SIZE/2)
      && (pos.x > BUTTON_L2_X - BUTTON_LEVEL_SIZE/2)
      && (pos.y > BUTTON_L2_Y - BUTTON_LEVEL_SIZE/2);
}

int32_t isLevel3(pixelPoint_t pos) {
  return (pos.x < BUTTON_L3_X + BUTTON_LEVEL_SIZE/2)
      && (pos.y < BUTTON_L3_Y + BUTTON_LEVEL_SIZE/2)
      && (pos.x > BUTTON_L3_X - BUTTON_LEVEL_SIZE/2)
      && (pos.y > BUTTON_L3_Y - BUTTON_LEVEL_SIZE/2);
}

int32_t isBack(pixelPoint_t pos) {
  return (pos.x < BUTTON_BACK_X + BUTTON_WIDTH/2)
      && (pos.y < BUTTON_BACK_Y + BUTTON_HEIGHT/2)
      && (pos.x > BUTTON_BACK_X - BUTTON_WIDTH/2)
      && (pos.y > BUTTON_BACK_Y - BUTTON_HEIGHT/2);
}
